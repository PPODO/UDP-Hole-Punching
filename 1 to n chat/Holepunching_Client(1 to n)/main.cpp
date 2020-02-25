#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#pragma comment(lib, "ws2_32.lib")

constexpr size_t BUFFER_SIZE = 1024;

std::vector<sockaddr_in> g_ClientList;
std::mutex g_ListMutex;

bool PeerInformationRegistration(SOCKET Socket) {
	sockaddr_in ServerAddress;
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(3550);
	ServerAddress.sin_addr.s_addr = inet_addr("59.5.200.189");

	if (sendto(Socket, "", 1, 0, reinterpret_cast<sockaddr*>(&ServerAddress), sizeof(sockaddr_in)) > 0) {
		return true;
	}
	return false;
}

void UpdateRegisteredPeerInformation(SOCKET Socket, std::stringstream& is) {
	int Count;
	is >> Count;

	for (int i = 0; i < Count; i++) {
		in_addr Address;
		u_short Port;
		is >> Address.S_un.S_addr >> Port;

		sockaddr_in NewClient;
		NewClient.sin_family = AF_INET;
		NewClient.sin_port = Port;
		NewClient.sin_addr = Address;

		std::cout << "Client(" << (i + 1) << ") : " << inet_ntoa(Address) << '\t' << Port << std::endl;
		sendto(Socket, "2", 1, 0, reinterpret_cast<sockaddr*>(&NewClient), sizeof(sockaddr_in));
		g_ListMutex.lock();
		g_ClientList.emplace_back(NewClient);
		g_ListMutex.unlock();
	}
}

void a(SOCKET a) {

}

void RecvThread(SOCKET Socket, std::atomic_bool* RunStatus) {
	while (*RunStatus) {
		sockaddr RecvAddress;
		int AddrLength = sizeof(sockaddr_in);
		if (char MessageBuffer[BUFFER_SIZE] = { "\0" }; recvfrom(Socket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLength) > 0) {
			std::stringstream is(MessageBuffer);
			int Type;
			is >> Type;

			if (Type == 0) {
				UpdateRegisteredPeerInformation(Socket, is);
			}
			else if (Type == 1) {
				std::string Buffer;
				is >> Buffer;
				std::cout << "Recv Message : " << Buffer << std::endl;
			}
			continue;
		}
		std::cout << "Recv Failure! - " << WSAGetLastError() << std::endl;
	}
}

int main() {
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed To Startup WSA!\n";
		return -1;
	}

	SOCKET ClientSocket;
	if ((ClientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
		std::cout << "Failed To Create Socket!\n";
		return -1;
	}

	std::atomic_bool RunStatus(true);
	std::thread hPeerInfoThread;
	if (PeerInformationRegistration(ClientSocket)) {
		hPeerInfoThread = std::thread(std::bind(RecvThread, ClientSocket, &RunStatus));
	}

	while (true) {
		std::string InputStream;
		std::getline(std::cin, InputStream);
		std::stringstream os;
		
		os << 1 << std::endl << InputStream << std::endl;
		g_ListMutex.lock();
		for (auto It : g_ClientList) {
			if (sendto(ClientSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<sockaddr*>(&It), sizeof(sockaddr_in)) <= 0) {
				std::cout << "Send Failure! - " << WSAGetLastError() << std::endl;
			}
		}
		g_ListMutex.unlock();
	}

	if (hPeerInfoThread.joinable()) {
		RunStatus.exchange(false);
		hPeerInfoThread.join();
	}
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}