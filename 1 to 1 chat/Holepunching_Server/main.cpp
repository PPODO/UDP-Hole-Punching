#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

constexpr size_t BUFFER_SIZE = 1024;
constexpr size_t MAX_CLIENT = 2;

int main() {
	WSADATA WinSockData;
	if (WSAStartup(WINSOCK_VERSION, &WinSockData) != 0) {
		std::cout << "Failed to Startup WSA!\n";
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
		std::cout << "Failed To Bind Address! - " << WSAGetLastError() << std::endl;
		return -1;
	}

	size_t Count = 0;
	sockaddr_in Clients[MAX_CLIENT];
	int AddrLength = sizeof(sockaddr_in);
	while (true) {
		sockaddr_in RecvAddress;
		if (char MessageBuffer[BUFFER_SIZE] = { "\0" }; recvfrom(ListenSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLength) > 0 && Count < MAX_CLIENT) {
			if (Count > 0) {
				{
					std::stringstream os;
					os << std::endl << RecvAddress.sin_addr.S_un.S_addr << std::endl << RecvAddress.sin_port;

					sendto(ListenSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<sockaddr*>(&Clients[0]), sizeof(sockaddr_in));
				}

				{
					std::stringstream os;
					os << std::endl << Clients[0].sin_addr.S_un.S_addr << std::endl << Clients[0].sin_port;

					sendto(ListenSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<sockaddr*>(&RecvAddress), sizeof(sockaddr_in));
				}
			}
			Clients[Count++] = RecvAddress;
			std::cout << "New Client : " << inet_ntoa(RecvAddress.sin_addr) << '\t' << RecvAddress.sin_port << std::endl;
		}
	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}