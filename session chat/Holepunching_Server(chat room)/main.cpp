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

enum EMESSAGETYPE : uint16_t {
	EMT_JOIN,
	EMT_FIND,
	EMT_CREATE,
	EMT_DISCONNECT
};

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

void JoinClient(SOCKET ListenSocket, std::stringstream& is, const sockaddr_in& RecvAddress, std::vector<std::map<unsigned short, sockaddr_in>>& SessionList, std::mutex& SessionMutex) {
	uint16_t SessionID;
	is >> SessionID;

	try {
		std::lock_guard<std::mutex> Lock(SessionMutex);
		
		auto Key = (RecvAddress.sin_addr.S_un.S_addr + RecvAddress.sin_port) % 100;
		if (auto Iterator = SessionList[SessionID].find(Key); Iterator == SessionList[SessionID].cend()) {
			{
				std::stringstream os;
				os << EMT_JOIN << std::endl << 1 << std::endl << RecvAddress.sin_addr.S_un.S_addr << std::endl << RecvAddress.sin_port << std::endl;

				for (auto Client : SessionList[SessionID]) {
					sendto(ListenSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<sockaddr*>(&Client.second), sizeof(sockaddr_in));
				}
			}

			{
				std::stringstream os;
				os << EMT_JOIN << std::endl << SessionList[SessionID].size() << std::endl;

				for (auto Client : SessionList[SessionID]) {
					os << Client.second.sin_addr.S_un.S_addr << std::endl << Client.second.sin_port << std::endl;
				}
				sendto(ListenSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<const sockaddr*>(&RecvAddress), sizeof(sockaddr_in));
			}
		}
		else {
			std::cout << "existing client!\n";
		}
	}
	catch (const std::out_of_range& exception) {
		std::cout << exception.what() << std::endl;
	}
}

void FindSession(std::stringstream& is) {

}

void CreateSession(std::stringstream& is) {

}

void DisconnectClient(SOCKET ListenSocket, std::stringstream& is, const sockaddr_in& RecvAddress, std::vector<std::map<unsigned short, sockaddr_in>>& SessionList, std::mutex& SessionMutex) {
	uint16_t SessionID;
	is >> SessionID;

	try {
		std::lock_guard<std::mutex> Lock(SessionMutex);

		auto Key = ((RecvAddress.sin_addr.S_un.S_addr + RecvAddress.sin_port) % 100);
		if (auto Iterator = SessionList[SessionID].find(Key); Iterator != SessionList[SessionID].cend()) {
			std::stringstream os;
			os << EMESSAGETYPE::EMT_DISCONNECT << std::endl << Key << std::endl;

			SessionList[SessionID].erase(Iterator);
			for (auto Client : SessionList[SessionID]) {
				sendto(ListenSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<sockaddr*>(&Client.second), sizeof(sockaddr_in));
			}
		}
	}
	catch (const std::out_of_range& exception) {
		std::cout << exception.what() << std::endl;
	}
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

	MessageQueue Queue;
	int AddrLength = sizeof(sockaddr_in);
	std::mutex SessionMutex;
	std::vector<std::map<unsigned short, sockaddr_in>> Sessions;
	std::atomic_bool RunState(true);
	std::thread WorkerThread([&ListenSocket, &RunState, &Queue, &Sessions, &SessionMutex]() {
		while (RunState) {
			auto Result = Queue.pop();
			std::stringstream is(std::get<0>(Result));
			uint16_t MessageType;
			is >> MessageType;

			switch (MessageType) {
			case EMESSAGETYPE::EMT_JOIN:
				JoinClient(is);
				break;
			case EMESSAGETYPE::EMT_FIND:
				FindSession(is);
				break;
			case EMESSAGETYPE::EMT_CREATE:
				CreateSession(is);
				break;
			case EMESSAGETYPE::EMT_DISCONNECT:
				DisconnectClient(ListenSocket, is, std::get<1>(Result), Sessions, SessionMutex);
				break;
			default:
				std::cout << "unknown message type!\n";
				break;
			}
		}
		});
	
	while (true) {
		sockaddr_in RecvAddress;
		if (char MessageBuffer[BUFFER_SIZE] = { "\0" }; recvfrom(ListenSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLength)) {
			Queue.push(MessageBuffer);
			std::cout << "Recv From : " << inet_ntoa(RecvAddress.sin_addr) << '\t' << RecvAddress.sin_port << std::endl;
		}
	}

	if (WorkerThread.joinable()) {
		RunState.exchange(false);
		WorkerThread.join();
	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}