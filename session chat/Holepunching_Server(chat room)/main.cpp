#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "Packets.h"
#pragma comment(lib, "ws2_32.lib")

constexpr size_t BUFFER_SIZE = 1024;

typedef struct SessionInfo {
	Packets::Types::CSessionInfo m_SessionInfo;
	std::unordered_map<unsigned long, Packets::Types::CSessionInfo::CUserInfo> m_UserInfo;

public:
	SessionInfo(const Packets::Types::CSessionInfo& SessionInfo, const std::pair<unsigned long, Packets::Types::CSessionInfo::CUserInfo>& HostUser) : m_SessionInfo(SessionInfo) { m_UserInfo.emplace(HostUser); };

};

std::stringstream ProcessingJoinSession(Packets::Types::CJoinPacket&& Packet, std::vector<SessionInfo>& SessionInfo) {
	using namespace Packets::Types;
	
	if (Packet.m_SessionInfo.m_SessionID < SessionInfo.size() && SessionInfo[Packet.m_SessionInfo.m_SessionID].m_SessionInfo.m_CurrentCount < SessionInfo[Packet.m_SessionInfo.m_SessionID].m_SessionInfo.m_MaximumCount) {
		auto& Session = SessionInfo[Packet.m_SessionInfo.m_SessionID];
		auto& UsersList = Session.m_SessionInfo.m_UsersInfo;
		for (auto UserInfo : UsersList) {
			Session.m_SessionInfo.m_CurrentCount++;
			UsersList.push_back(Packets::Types::CSessionInfo::CUserInfo(UserInfo.m_Port, UserInfo.m_Address, UserInfo.m_Nickname));
		}
		return operator<<(std::stringstream(), CJoinPacket(Session.m_SessionInfo));
	}
	return operator<<(std::stringstream(), CJoinPacket(Packets::ErrorCode::EMAXIMUMNUMBER));
}

std::stringstream ProcessingFindSession(Packets::Types::CFindPacket&& Packet, const std::vector<SessionInfo>& SessionInfo) {
	using namespace Packets::Types;

	std::vector<CSessionInfo> SessionList;
	for (auto Sessions : SessionInfo) {
		SessionList.emplace_back(Sessions.m_SessionInfo);
	}

	return operator<<(std::stringstream(), CFindPacket(SessionList));
}

std::stringstream ProcessingCreateSession(Packets::Types::CCreatePacket&& Packet, std::vector<SessionInfo>& SessionInfo, const unsigned long Key, const sockaddr_in& RecvAddress) {
	using namespace Packets::Types;

	Packet.m_SessionInformation.m_SessionID = SessionInfo.size();
	Packet.m_SessionInformation.m_CurrentCount = 1;
	SessionInfo.emplace_back(Packet.m_SessionInformation, std::make_pair(Key, Packets::Types::CSessionInfo::CUserInfo(RecvAddress.sin_port, RecvAddress.sin_addr.S_un.S_addr, 
	Packet.m_SessionInformation.m_UsersInfo.front().m_Nickname)));

	return operator<<(std::stringstream(), CJoinPacket(Packet.m_SessionInformation));
}

int main() {
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed To Startup WSA!\n";
		return -1;
	}

	SOCKET ListenSocket;
	if ((ListenSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		std::cout << "Failed To Create Socket!\n";
		return -1;
	}

	sockaddr_in BindAddress;
	BindAddress.sin_family = AF_INET;
	BindAddress.sin_port = htons(3550);
	BindAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
	if (bind(ListenSocket, reinterpret_cast<sockaddr*>(&BindAddress), sizeof(sockaddr_in)) == SOCKET_ERROR) {
		std::cout << "Failed To Bind Socket!\n";
		return -1;
	}

	std::unordered_map<unsigned long, sockaddr_in> ConnectedUserInfo;
	std::vector<SessionInfo> SessionsInfo;
	int AddrLength = sizeof(sockaddr_in);
	while (true) {
		sockaddr_in RecvAddress;
		int RecvBytes = 0;
		if (char MessageBuffer[BUFFER_SIZE] = { "\0" }; (RecvBytes = recvfrom(ListenSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLength)) > 0) {
			Packets::Types::CPacket Packet{ std::stringstream(MessageBuffer) };

			switch (auto Key = (RecvAddress.sin_addr.S_un.S_addr + RecvAddress.sin_port) % 100; Packet.m_MessageType) {
			case Packets::MessageType::EMT_CONNECT:
				if (ConnectedUserInfo.find(Key) == ConnectedUserInfo.cend()) {
					ConnectedUserInfo.insert(std::make_pair(Key, RecvAddress));
					std::cout << "Connect!\n";
				}
				break;
			case Packets::MessageType::EMT_JOIN:
			{
				auto Result = ProcessingJoinSession(Packets::Types::CJoinPacket{ std::stringstream(std::string(MessageBuffer, RecvBytes)) }, SessionsInfo);
				sendto(ListenSocket, Result.str().c_str(), Result.str().length(), 0, reinterpret_cast<sockaddr*>(&RecvAddress), sizeof(sockaddr_in));
			}
				break;
			case Packets::MessageType::EMT_FIND:
			{
				auto Result = ProcessingFindSession(Packets::Types::CFindPacket{ std::stringstream(std::string(MessageBuffer, RecvBytes)) }, SessionsInfo);
				sendto(ListenSocket, Result.str().c_str(), Result.str().length(), 0, reinterpret_cast<sockaddr*>(&RecvAddress), sizeof(sockaddr_in));
			}
				break;
			case Packets::MessageType::EMT_CREATE:
			{
				auto Result = ProcessingCreateSession(Packets::Types::CCreatePacket{ std::stringstream(std::string(MessageBuffer, RecvBytes)) }, SessionsInfo, Key, RecvAddress);
				sendto(ListenSocket, Result.str().c_str(), Result.str().length(), 0, reinterpret_cast<sockaddr*>(&RecvAddress), sizeof(sockaddr_in));
			}
				break;
			case Packets::MessageType::EMT_DISCONNECT:
				if (auto It = ConnectedUserInfo.find(Key); It != ConnectedUserInfo.cend()) {
					ConnectedUserInfo.erase(It);
					std::cout << "Disconnect!\n";
				}
				break;
			default:
				std::cout << "Unknown Message Type : " << Packet.m_MessageType << std::endl;
				break;
			}
		}
	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}