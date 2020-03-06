#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <queue>
#include <map>
#pragma comment(lib, "ws2_32.lib")

constexpr size_t BUFFER_SIZE = 1024;

#pragma once
#include <iostream>
#include <sstream>
#include <vector>

namespace Packets {
	namespace MessageType {
		enum EMESSAGETYPE : uint16_t {
			EMT_CONNECT = 1,
			EMT_JOIN,
			EMT_FIND,
			EMT_CREATE,
			EMT_DISCONNECT
		};
	}

	namespace Types {
		struct CSessionInfo {
		public:
			std::string m_SessionName;
			uint16_t m_SessionID;
			uint16_t m_MaximumCount;
			uint16_t m_CurrentCount;
			bool m_bUsePassword;

		public:
			friend std::ostream& operator<<(std::ostream& os, const CSessionInfo& SessionInfo) {
				size_t Length = SessionInfo.m_SessionName.length();
				os.write(reinterpret_cast<const char*>(&Length), sizeof(size_t));
				os.write(SessionInfo.m_SessionName.c_str(), SessionInfo.m_SessionName.length());
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_SessionID), sizeof(SessionInfo.m_SessionID));
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_MaximumCount), sizeof(SessionInfo.m_MaximumCount));
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_CurrentCount), sizeof(SessionInfo.m_CurrentCount));
				os.write(reinterpret_cast<const char*>(&SessionInfo.m_bUsePassword), sizeof(SessionInfo.m_bUsePassword));

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CSessionInfo& SessionInfo) {
				size_t Length;
				is.read(reinterpret_cast<char*>(&Length), sizeof(size_t));
				char* TempBuffer = new char[Length];
				is.read(TempBuffer, Length);
				SessionInfo.m_SessionName = std::string(TempBuffer);
				delete[] TempBuffer;

				is.read(reinterpret_cast<char*>(&SessionInfo.m_SessionID), sizeof(SessionInfo.m_SessionID));
				is.read(reinterpret_cast<char*>(&SessionInfo.m_MaximumCount), sizeof(SessionInfo.m_MaximumCount));
				is.read(reinterpret_cast<char*>(&SessionInfo.m_CurrentCount), sizeof(SessionInfo.m_CurrentCount));
				is.read(reinterpret_cast<char*>(&SessionInfo.m_bUsePassword), sizeof(SessionInfo.m_bUsePassword));

				return is;
			}

		};

		struct CPacket {
		public:
			unsigned short m_MessageType;

		public:
			CPacket(const unsigned short _MessageType) : m_MessageType(_MessageType) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CPacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CPacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));

				return is;
			}

		};

		struct CJoinPacket : public CPacket {
		public:
			uint16_t m_SessionID;

		public:
			CJoinPacket(uint16_t SessionID) : CPacket(MessageType::EMESSAGETYPE::EMT_JOIN), m_SessionID(SessionID) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CJoinPacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				os.write(reinterpret_cast<const char*>(&Packet.m_SessionID), sizeof(Packet.m_SessionID));

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CJoinPacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				is.read(reinterpret_cast<char*>(&Packet.m_SessionID), sizeof(Packet.m_SessionID));

				return is;
			}

		};

		struct CFindPacket : public CPacket {
		public:
			std::vector<CSessionInfo> m_SessionInformation;

		public:
			CFindPacket() : CPacket(MessageType::EMESSAGETYPE::EMT_FIND) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CFindPacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				size_t Size = Packet.m_SessionInformation.size();
				os.write(reinterpret_cast<const char*>(&Size), sizeof(size_t));
				for (const auto& It : Packet.m_SessionInformation) {
					os << It;
				}

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CFindPacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				size_t Size;
				is.read(reinterpret_cast<char*>(&Size), sizeof(Size));
				for (int i = 0; i < Size; i++) {
					CSessionInfo Info;
					is >> Info;
					Packet.m_SessionInformation.insert(Packet.m_SessionInformation.begin(), 0, Info);
				}

				return is;
			}

		};


		struct CCreatePacket : public CPacket {
		public:
			CSessionInfo m_SessionInformation;

		public:
			CCreatePacket(const CSessionInfo& SessionInfo) : CPacket(MessageType::EMESSAGETYPE::EMT_CREATE), m_SessionInformation(SessionInfo) {};

		public:
			friend std::ostream& operator<<(std::ostream& os, const CCreatePacket& Packet) {
				os.write(reinterpret_cast<const char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				os << Packet.m_SessionInformation;

				return os;
			}

			friend std::istream& operator>>(std::istream& is, CCreatePacket& Packet) {
				is.read(reinterpret_cast<char*>(&Packet.m_MessageType), sizeof(Packet.m_MessageType));
				is >> Packet.m_SessionInformation;

				return is;
			}

		};

	}
}

class MessageQueue {
private:
	std::mutex m_QueueMutex;
	std::queue<std::tuple<std::string, sockaddr_in>> m_Queue;

public:
	explicit MessageQueue() noexcept {};
	~MessageQueue() noexcept {};

public:
	void push(const std::string& Message, const sockaddr_in& RecvAddress) {
		std::lock_guard<std::mutex> Lock(m_QueueMutex);
		m_Queue.push(std::make_tuple(Message, RecvAddress));
	}
	
	auto pop() {
		std::lock_guard<std::mutex> Lock(m_QueueMutex);
		auto Result = m_Queue.front();
		m_Queue.pop();

		return Result;
	}

	bool IsEmpty() {
		std::lock_guard<std::mutex> Lock(m_QueueMutex);

		return m_Queue.empty();
	}
};

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

	int AddrLength = sizeof(sockaddr_in);
	while (true) {
		sockaddr_in RecvAddress;
		if (char MessageBuffer[BUFFER_SIZE] = { "\0" }; recvfrom(ListenSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLength) > 0) {
			std::cout << "Recv From : " << inet_ntoa(RecvAddress.sin_addr) << '\t' << RecvAddress.sin_port << std::endl;
			sendto(ListenSocket, MessageBuffer, strlen(MessageBuffer), 0, reinterpret_cast<sockaddr*>(&RecvAddress), sizeof(sockaddr_in));
		}
	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}