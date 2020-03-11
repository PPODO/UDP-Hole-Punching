#include "FindSessionForm.h"
#include "MainForm.h"

namespace HolepunchingClientchatroom {
	System::Void FindSessionForm::FindSessionForm_Load(System::Object^ sender, System::EventArgs^ e) {
		if (this->Owner->Validate()) {
			this->Owner->Enabled = false;
			
			std::stringstream Stream;
			auto Owner = dynamic_cast<MainForm^>(this->Owner);
			Owner->FindDelegate = gcnew FindSessionPacketDelegate(this, &FindSessionForm::PacketProcessing);
			Stream << Packets::Types::CFindPacket();
			AsyncSocket::SendTo(Owner->SocketObject, gcnew String(Stream.str().c_str(), 0, Stream.str().length()));
		}
	}

	System::Void FindSessionForm::FindSessionForm_Closed(System::Object^ sender, System::EventArgs^ e) {
		if (this->Owner->Validate()) {
			this->Owner->Enabled = true;
		}
	}

	System::Void FindSessionForm::PacketProcessing(const Packets::Types::CFindPacket& Packet) {

	}
}