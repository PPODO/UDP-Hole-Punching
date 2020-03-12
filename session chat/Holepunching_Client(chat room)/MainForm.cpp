#include "MainForm.h"
#include "FindSessionForm.h"
#include "CreateSessionForm.h"
#include "ChatroomForm.h"
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
		ModalObject->ShowDialog();
	}

	System::Void MainForm::Create_Click(System::Object^ sender, System::EventArgs^ e) {
		CreateSessionForm^ ModalObject = gcnew CreateSessionForm{ this };
		ModalObject->ShowDialog();
	}

	System::Void MainForm::JoinPacket_Delegate(array<unsigned char>^ Buffer) {
		pin_ptr<unsigned char> Ptr = &Buffer[0];
		Packets::Types::CJoinPacket Packet{ std::stringstream(std::string(reinterpret_cast<char*>(Ptr), Buffer->Length)) };

		this->Hide();
		ChatroomForm^ ModalObject = gcnew ChatroomForm(gcnew String(Packet.m_SessionInfo.m_SessionName.c_str()), this);
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
			{
				if (Packet.m_ErrorCode == Packets::ErrorCode::ESUCCEED) {
					this->Invoke(m_JoinPacketDelegate, SocketObj->m_ReceiveBuffer);
				}
				else {
					System::Windows::Forms::MessageBox::Show("You cannot participate in the session! (Maximum number of people)", "Warning", System::Windows::Forms::MessageBoxButtons::OK);
				}
			}
				break;
			case MessageType::EMESSAGETYPE::EMT_FIND:
				if (m_FindPacketDelegate != nullptr) {
					this->Invoke(m_FindPacketDelegate, SocketObj->m_ReceiveBuffer);
				}
				break;
			case MessageType::EMESSAGETYPE::EMT_CREATE:
				if (m_CreatePacketDelegate != nullptr) {
					this->Invoke(m_CreatePacketDelegate, SocketObj->m_ReceiveBuffer);
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