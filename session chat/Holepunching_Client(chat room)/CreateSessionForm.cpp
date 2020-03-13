#include "CreateSessionForm.h"
#include "ChatroomForm.h"
#include "NicknameInputForm.h"
#include "MainForm.h"
#include <msclr/marshal_cppstd.h>

namespace HolepunchingClientchatroom {
	System::Void CreateSessionForm::CreateSessionForm_Load(System::Object^ sender, System::EventArgs^ e) {
		if (this->Owner->Validate()) {
			auto Owner = dynamic_cast<MainForm^>(this->Owner);
			Owner->CreateDelegate = gcnew CreateSessionPacketDelegate(this, &CreateSessionForm::PacketProcessing);
			Owner->JoinDelegate += gcnew JoinSessionPacketDelegate(this, &CreateSessionForm::JoinSession_Delegate);
		}
	}
	
	System::Void CreateSessionForm::CreateSessionForm_Closed(System::Object^ sender, System::EventArgs^ e) {
		if (this->Owner->Validate()) {
			auto Owner = dynamic_cast<MainForm^>(this->Owner);
			Owner->CreateDelegate -= gcnew CreateSessionPacketDelegate(this, &CreateSessionForm::PacketProcessing);
			Owner->JoinDelegate -= gcnew JoinSessionPacketDelegate(this, &CreateSessionForm::JoinSession_Delegate);
		}
	}

	System::Void CreateSessionForm::UsePasswordCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		if (this->UsePasswordCheckBox->Checked) {
			this->PasswordTextBox->Enabled = true;
		}
		else {
			this->PasswordTextBox->Enabled = false;
		}
	}

	System::Void CreateSessionForm::ConfirmButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (this->SessionNameTextBox->TextLength <= 0) {
			System::Windows::Forms::MessageBox::Show("Session Name Must Be At Least 0 Characters Long!", "Warning", System::Windows::Forms::MessageBoxButtons::OK);
			return;
		}
		if (this->UsePasswordCheckBox->Checked && this->PasswordTextBox->TextLength <= 0) {
			System::Windows::Forms::MessageBox::Show("Password Must Be At Least 0 Characters Long!", "Warning", System::Windows::Forms::MessageBoxButtons::OK);
			return;
		}

		NicknameInputForm^ ModalObject = gcnew NicknameInputForm(this, gcnew EnterNicknameDelegate(this, &CreateSessionForm::EnterNickname_Delegate));
		ModalObject->ShowDialog();
	}

	System::Void CreateSessionForm::PacketProcessing(array<unsigned char>^ Buffer) {


	}

	System::Void CreateSessionForm::JoinSession_Delegate(array<unsigned char>^ Buffer) {
		this->Close();
	}

	System::Void CreateSessionForm::EnterNickname_Delegate(System::String^ nickname) {
		using namespace msclr::interop;

		std::stringstream Stream;
		Stream << Packets::Types::CCreatePacket(Packets::Types::CSessionInfo(marshal_as<std::string>(this->SessionNameTextBox->Text), 
		this->MaxCountComboBox->SelectedIndex + 2, this->UsePasswordCheckBox->Checked, marshal_as<std::string>(this->PasswordTextBox->Text),
		Packets::Types::CSessionInfo::CUserInfo(nickname)));

		AsyncSocket::SendTo(dynamic_cast<MainForm^>(this->Owner)->SocketObject, gcnew String(Stream.str().c_str(), 0, Stream.str().length()));
	}
}