#include "MainForm.h"
#include "FindSessionForm.h"
#include "CreateSessionForm.h"
#include "Packets.h"

namespace HolepunchingClientchatroom {
	System::Void MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
		std::stringstream Stream;
		Stream << Packets::Types::CPacket(Packets::MessageType::EMESSAGETYPE::EMT_CONNECT);

		AsyncSocket::SendTo(m_UdpSocket, gcnew String(Stream.str().c_str()));
	}

	System::Void MainForm::MainForm_Closed(System::Object^ sender, System::EventArgs^ e) {
		std::stringstream Stream;
		Stream << Packets::Types::CPacket(Packets::MessageType::EMESSAGETYPE::EMT_DISCONNECT);

		AsyncSocket::SendTo(m_UdpSocket, gcnew String(Stream.str().c_str()));
	}

	System::Void MainForm::Find_Click(System::Object^ sender, System::EventArgs^ e) {
		FindSessionForm^ ModalObject = gcnew FindSessionForm{ this };
		ModalObject->Show();
	}

	System::Void MainForm::Create_Click(System::Object^ sender, System::EventArgs^ e) {
		CreateSessionForm^ ModalObject = gcnew CreateSessionForm{ this };
		ModalObject->Show();
	}

	System::Void MainForm::HandleDataReceive(System::IAsyncResult^ Result) {
		using namespace Packets;

		auto SocketObj = safe_cast<AsyncSocket::UdpAsyncObject^>(Result->AsyncState);
		int ReceiveBytes = 0;

		if (SocketObj && (ReceiveBytes = SocketObj->m_Socket->EndReceiveFrom(Result, reinterpret_cast<EndPoint^%>(SocketObj->m_RemoteAddress))) > 0) {
			pin_ptr<unsigned char> Ptr(&SocketObj->m_ReceiveBuffer[0]);
			Types::CPacket Packet{ std::stringstream(reinterpret_cast<char*>(Ptr)) };

			switch (Packet.m_MessageType) {
			case MessageType::EMESSAGETYPE::EMT_JOIN:
				/*
				*/
				break;
			case MessageType::EMESSAGETYPE::EMT_FIND:
				if (m_FindPacketDelegate != nullptr) {
					m_FindPacketDelegate(Types::CFindPacket{ std::stringstream(std::string(reinterpret_cast<char*>(Ptr), ReceiveBytes)) });
				}
				break;
			case MessageType::EMESSAGETYPE::EMT_CREATE:
				if (m_CreatePacketDelegate != nullptr) {
					m_CreatePacketDelegate(Types::CCreatePacket{ std::stringstream(std::string(reinterpret_cast<char*>(Ptr), ReceiveBytes)) });
				}
				break;
			}
		}
		else {
			Console::WriteLine("Receive Failure!");
		}
		AsyncSocket::ReceiveFrom(m_UdpSocket);
	}
}