#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <memory>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

ref class CSocketClass {
private:
	WSADATA* m_WinSockData;

private:
	SOCKET m_ClientSocket;
	sockaddr_in* m_ServerAddress;

public:
	explicit CSocketClass() : m_WinSockData(new WSADATA), m_ServerAddress(new sockaddr_in){
		if (!m_ServerAddress || !m_WinSockData || WSAStartup(WINSOCK_VERSION, m_WinSockData) != 0) {
			throw "WSA Initialization Failure!\n";
		}
	}

	~CSocketClass() {
		WSACleanup();

		if (m_ServerAddress) {
			delete m_ServerAddress;
		}
		if (m_WinSockData) {
			delete m_WinSockData;
		}
	}

public:
	bool Initialize() {
		if ((m_ClientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
			std::cout << "Socket Initialization Failure!\n";
			return false;
		}
	}

public:
	bool Send() {

	}


};