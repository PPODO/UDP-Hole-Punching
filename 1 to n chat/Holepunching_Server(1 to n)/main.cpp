#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#pragma comment(lib, "ws2_32.lib")

constexpr size_t BUFFER_SIZE = 1024;

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

	std::map<u_long, sockaddr_in> ClientInformation;
	int AddrLength = sizeof(sockaddr_in);
	while (true) {
		sockaddr_in RecvAddress;
		if (char MessageBuffer[BUFFER_SIZE] = { "\0" }; recvfrom(ListenSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLength) >= 0) {
			if (auto Key = (RecvAddress.sin_addr.S_un.S_addr + RecvAddress.sin_port) % 100; ClientInformation.find(Key) == ClientInformation.cend()) {
				{
					std::stringstream os;
					os << 0 << std::endl << 1 << std::endl << RecvAddress.sin_addr.S_un.S_addr << std::endl << RecvAddress.sin_port << std::endl;
					
					for (auto It : ClientInformation) {
						sendto(ListenSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<sockaddr*>(&It.second), sizeof(sockaddr_in));
					}
				}

				{
					std::stringstream os;
					os << 0 << std::endl << ClientInformation.size() << std::endl;
					
					for (auto It : ClientInformation) {
						os << It.second.sin_addr.S_un.S_addr << std::endl << It.second.sin_port << std::endl;
					}
					sendto(ListenSocket, os.str().c_str(), os.str().length(), 0, reinterpret_cast<sockaddr*>(&RecvAddress), sizeof(sockaddr_in));
				}

				ClientInformation.emplace(Key, RecvAddress);
				std::cout << "New Client : " << inet_ntoa(RecvAddress.sin_addr) << '\t' << RecvAddress.sin_port << std::endl;
			}
		}
	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}