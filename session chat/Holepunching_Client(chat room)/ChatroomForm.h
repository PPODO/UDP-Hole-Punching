#pragma once

namespace HolepunchingClientchatroom {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ChatroomForm : public System::Windows::Forms::Form {
	public:
		ChatroomForm(String^ SessionName, System::Windows::Forms::Form^ Owner) {
			InitializeComponent(SessionName, Owner);
		}

	protected:
		~ChatroomForm() {
			if (components) {
				delete components;
			}
		}

	private:
		System::ComponentModel::Container^ components;

	private:
		void InitializeComponent(String^ SessionName, System::Windows::Forms::Form^ Owner) {
			this->Owner = Owner;

			this->SuspendLayout();
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1080, 600);
			this->Name = L"ChatroomForm";
			this->Text = SessionName;
			this->Load += gcnew System::EventHandler(this, &ChatroomForm::ChatroomForm_Load);
			this->Closed += gcnew System::EventHandler(this, &ChatroomForm::ChatroomForm_Closed);
			this->ResumeLayout(false);
		}

	private: 
		System::Void ChatroomForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void ChatroomForm_Closed(System::Object^ sender, System::EventArgs^ e);

	};
}
