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
		System::Windows::Forms::RichTextBox^ ChattingTextBox;
		System::Windows::Forms::RichTextBox^ ChatInputTextBox;
		System::Windows::Forms::ListView^ UserListView;

	private:
		void InitializeComponent(String^ SessionName, System::Windows::Forms::Form^ Owner) {
			this->ChattingTextBox = gcnew System::Windows::Forms::RichTextBox();
			this->ChatInputTextBox = gcnew System::Windows::Forms::RichTextBox();
			this->UserListView = gcnew System::Windows::Forms::ListView();

			{
				this->ChattingTextBox->Name = "Chatting TextBox";
				this->ChattingTextBox->Size = System::Drawing::Size(1000, 75);
				this->ChattingTextBox->Location = System::Drawing::Point((1080 / 2) - 500, 500);
				this->ChattingTextBox->TabIndex = 3;
			}

			{
				this->ChatInputTextBox->BackColor = System::Drawing::SystemColors::ActiveBorder;
				this->ChatInputTextBox->Name = "Chat Input TextBox";
				this->ChatInputTextBox->Size = System::Drawing::Size(850, 475);
				this->ChatInputTextBox->Location = System::Drawing::Point(40, 15);
				this->ChatInputTextBox->ReadOnly = true;
				this->ChatInputTextBox->TabIndex = 2;
			}

			{
				this->UserListView->HideSelection = false;
				this->UserListView->Name = L"User ListView";
				this->UserListView->Size = System::Drawing::Size(125, 475);
				this->UserListView->Location = System::Drawing::Point(915, 15);
				this->UserListView->UseCompatibleStateImageBehavior = false;
				this->UserListView->TabIndex = 0;
				this->UserListView->View = System::Windows::Forms::View::Details;
				this->UserListView->Columns->Add(L"Name", 125, System::Windows::Forms::HorizontalAlignment::Center);
			}

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

			this->Controls->Add(ChattingTextBox);
			this->Controls->Add(ChatInputTextBox);
			this->Controls->Add(UserListView);
		}

	private: 
		System::Void ChatroomForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void ChatroomForm_Closed(System::Object^ sender, System::EventArgs^ e);

	};
}
