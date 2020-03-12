#include "ChatroomForm.h"
#include "MainForm.h"

namespace HolepunchingClientchatroom {
	System::Void ChatroomForm::ChatroomForm_Load(System::Object^ sender, System::EventArgs^ e) {
		return System::Void();
	}

	System::Void ChatroomForm::ChatroomForm_Closed(System::Object^ sender, System::EventArgs^ e) {
		if (this->Owner->Validate()) {
			dynamic_cast<MainForm^>(this->Owner)->Show();
		}
	}
}