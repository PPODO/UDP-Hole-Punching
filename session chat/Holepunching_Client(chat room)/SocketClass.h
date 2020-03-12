#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <memory>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace System::Net;
using namespace System::Net::Sockets;

constexpr int BUFFER_LENGTH = 1024;

namespace AsyncSocket {
	ref struct UdpAsyncObject;
	inline void ReceiveFrom(UdpAsyncObject^% Object);

	ref struct UdpAsyncObject {
	public:
		Socket^ m_Socket;
		IPEndPoint^ const m_ServerAddress;
		IPEndPoint^ m_RemoteAddress;

	public:
		array<unsigned char>^ m_ReceiveBuffer;

	public:
		System::AsyncCallback^ const m_CallbackObj;

	public:
		UdpAsyncObject(IPEndPoint^ const ServerAddress, System::AsyncCallback^ const AsyncCallbackObj) : m_Socket(gcnew Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp)), m_ServerAddress(ServerAddress), m_RemoteAddress(gcnew IPEndPoint(0, 0)), m_ReceiveBuffer(gcnew array<unsigned char>(BUFFER_LENGTH)), m_CallbackObj(AsyncCallbackObj) {
			if (!AsyncCallbackObj) {
				System::Console::WriteLine("Invalid Callback Object!");
				exit(-1);
			}
			m_Socket->Bind(gcnew IPEndPoint(IPAddress::Any, 0));
			ReceiveFrom(this);
		};

	};

	inline void ReceiveFrom(UdpAsyncObject^% Object) {
		Object->m_Socket->BeginReceiveFrom(Object->m_ReceiveBuffer, 0, BUFFER_LENGTH, SocketFlags::None, reinterpret_cast<EndPoint^%>(Object->m_RemoteAddress), Object->m_CallbackObj, Object);
	}

	inline bool SendTo(UdpAsyncObject^ Object, IPEndPoint^ const SendAddress, System::String^ const Data) {
		return Object->m_Socket->SendTo(System::Text::Encoding::ASCII->GetBytes(Data), SocketFlags::None, SendAddress) > 0 ? true : false;
	}

	inline bool SendTo(UdpAsyncObject^ Object, System::String^ const Data) {
		return Object->m_Socket->SendTo(System::Text::Encoding::ASCII->GetBytes(Data), SocketFlags::None, Object->m_ServerAddress) > 0 ? true : false;
	}
}