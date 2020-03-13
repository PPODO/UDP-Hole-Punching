#include "FindSessionForm.h"
#include "NicknameInputForm.h"
#include "MainForm.h"

namespace HolepunchingClientchatroom {
	System::Void FindSessionForm::FindSessionForm_Load(System::Object^ sender, System::EventArgs^ e) {
		if (this->Owner->Validate()) {
			std::stringstream Stream;
			auto Owner = dynamic_cast<MainForm^>(this->Owner);
			Owner->FindDelegate = gcnew FindSessionPacketDelegate(this, &FindSessionForm::PacketProcessing);
			Owner->JoinDelegate += gcnew JoinSessionPacketDelegate(this, &FindSessionForm::JoinSession_Delegate);
			Stream << Packets::Types::CFindPacket();
			AsyncSocket::SendTo(Owner->SocketObject, gcnew String(Stream.str().c_str(), 0, Stream.str().length()));
		}
	}

	System::Void FindSessionForm::FindSessionForm_Closed(System::Object^ sender, System::EventArgs^ e) {
		if (this->Owner->Validate()) {
			auto Owner = dynamic_cast<MainForm^>(this->Owner);
			Owner->FindDelegate -= gcnew FindSessionPacketDelegate(this, &FindSessionForm::PacketProcessing);
			Owner->JoinDelegate -= gcnew JoinSessionPacketDelegate(this, &FindSessionForm::JoinSession_Delegate);
		}
	}

	System::Void FindSessionForm::JoinButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (this->SessionList->FocusedItem != nullptr) {
			NicknameInputForm^ ModalObject = gcnew NicknameInputForm(this, gcnew EnterNicknameDelegate(this, &FindSessionForm::EnterNickname_Delegate));
			ModalObject->ShowDialog();
		}
	}

	System::Void FindSessionForm::PacketProcessing(array<unsigned char>^ Buffer) {
		pin_ptr<unsigned char> Ptr = &Buffer[0];
		Packets::Types::CFindPacket Packet{ std::stringstream(std::string(reinterpret_cast<char*>(Ptr), Buffer->Length)) };
		
		for (auto Session : Packet.m_SessionInformation) {
			System::Windows::Forms::ListViewItem^ Item = gcnew System::Windows::Forms::ListViewItem(gcnew array<String^>(5) {
				Session.m_SessionID.ToString(), gcnew String(Session.m_SessionName.c_str()), Session.m_CurrentCount.ToString(),
				Session.m_MaximumCount.ToString(), Session.m_bUsePassword.ToString()
			});
			this->SessionList->Items->Add(Item);
		}
	}

	System::Void FindSessionForm::JoinSession_Delegate(array<unsigned char>^ Buffer) {
		this->Close();
	}

	System::Void FindSessionForm::EnterNickname_Delegate(System::String^ nickname) {
		auto Item = this->SessionList->Items[this->SessionList->FocusedItem->Index];

		std::stringstream Stream;
		Stream << Packets::Types::CJoinPacket(Packets::Types::CSessionInfo(Convert::ToInt16(Item->Text), Packets::Types::CSessionInfo::CUserInfo(nickname)));
		AsyncSocket::SendTo(dynamic_cast<MainForm^>(this->Owner)->SocketObject, gcnew String(Stream.str().c_str(), 0, Stream.str().length()));
	}
}