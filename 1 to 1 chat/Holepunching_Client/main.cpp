#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

constexpr size_t BUFFER_SIZE = 1024;

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

	sockaddr_in SendAddress;
	SendAddress.sin_family = AF_INET;
	SendAddress.sin_addr.s_addr = inet_addr("59.5.200.189");
	SendAddress.sin_port = htons(3550);
	sendto(ClientSocket, "\0", 1, 0, reinterpret_cast<sockaddr*>(&SendAddress), sizeof(sockaddr_in));

	char MessageBuffer[BUFFER_SIZE] = { "\0" };
	sockaddr RecvAddr;
	int AddrLength = sizeof(sockaddr);
	recvfrom(ClientSocket, MessageBuffer, BUFFER_SIZE, 0, &RecvAddr, &AddrLength);

	std::stringstream is(MessageBuffer);
	sockaddr_in OtherClientAddress;
	unsigned short Port;
	in_addr Address;

	is >> Address.S_un.S_addr >> Port;
	OtherClientAddress.sin_family = AF_INET;
	OtherClientAddress.sin_addr = Address;
	OtherClientAddress.sin_port = Port;

	sendto(ClientSocket, "\0", 1, 0, reinterpret_cast<sockaddr*>(&OtherClientAddress), sizeof(sockaddr_in));
	std::cout << inet_ntoa(Address) << '\t' << Port << std::endl;


	std::thread RecvThread([&]() {
		while (true) {
			sockaddr_in RecvAddress;
			if (char MessageBuffer[BUFFER_SIZE] = { "\0" }; recvfrom(ClientSocket, MessageBuffer, BUFFER_SIZE, 0, reinterpret_cast<sockaddr*>(&RecvAddress), &AddrLength) > 0) {
				std::cout << "Recv : " << MessageBuffer << std::endl;
				continue;
			}
			std::cout << "Recv Failure!\n";
		}
		});

	while (true) {
		std::string InputStream;
		std::getline(std::cin, InputStream);

		if (sendto(ClientSocket, InputStream.c_str(), InputStream.length(), 0, reinterpret_cast<sockaddr*>(&OtherClientAddress), sizeof(sockaddr_in)) <= 0) {
			std::cout << "Send Failure! - " << WSAGetLastError() << std::endl;
			continue;
		}
		std::cout << "Send!\n";
	}

	if (RecvThread.joinable()) {
		RecvThread.join();
	}
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}