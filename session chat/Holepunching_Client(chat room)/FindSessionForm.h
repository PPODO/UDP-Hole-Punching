#pragma once
#include "Packets.h"

namespace HolepunchingClientchatroom {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	ref class MainForm;

	public ref class FindSessionForm : public System::Windows::Forms::Form {
	public:
		FindSessionForm(System::Windows::Forms::Form^ Owner) {
			InitializeComponent(Owner);
		}

	protected:
		~FindSessionForm() {
			if (components) {
				delete components;
			}
		}

	private:
		System::ComponentModel::Container^ components;

	private:
		System::Windows::Forms::ListView^ SessionList;
		System::Windows::Forms::ColumnHeader^ IDHeader;
		System::Windows::Forms::ColumnHeader^ NameHeader;
		System::Windows::Forms::ColumnHeader^ CurrentHeader;
		System::Windows::Forms::ColumnHeader^ MaxHeader;
		System::Windows::Forms::ColumnHeader^ UsePasswordHeader;

	private:
		System::Windows::Forms::Button^ JoinButton;

	private:

		void InitializeComponent(System::Windows::Forms::Form^ Owner) {
			SessionList = gcnew System::Windows::Forms::ListView();
			IDHeader = gcnew System::Windows::Forms::ColumnHeader();
			NameHeader = gcnew System::Windows::Forms::ColumnHeader();
			CurrentHeader = gcnew System::Windows::Forms::ColumnHeader();
			MaxHeader = gcnew System::Windows::Forms::ColumnHeader();
			UsePasswordHeader = gcnew System::Windows::Forms::ColumnHeader();
			JoinButton = gcnew System::Windows::Forms::Button();

			this->Owner = Owner;
			this->SuspendLayout();
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(600, 400);
			this->MaximizeBox = false;
			this->StartPosition = FormStartPosition::Manual;
			this->Location = System::Drawing::Point(Owner->Location.X + (Owner->ClientSize.Width / 2) - (this->ClientSize.Width / 2), Owner->Location.Y + (Owner->ClientSize.Height / 2) - (this->ClientSize.Height / 2));
			this->Name = L"FindSessionForm";
			this->Text = L"Find Session";
			this->Load += gcnew System::EventHandler(this, &FindSessionForm::FindSessionForm_Load);
			this->Closed += gcnew System::EventHandler(this, &FindSessionForm::FindSessionForm_Closed);
			this->ResumeLayout(false);

			{
				this->IDHeader->Text = L"Session ID";
				this->NameHeader->Text = L"Name";
				this->CurrentHeader->Text = L"Current";
				this->MaxHeader->Text = L"Max";
				this->UsePasswordHeader->Text = L"Use Password";

				const int Width = 550 / 5;
				this->IDHeader->Width = Width;
				this->NameHeader->Width = Width;
				this->CurrentHeader->Width = Width;
				this->MaxHeader->Width = Width;
				this->UsePasswordHeader->Width = Width;

				this->IDHeader->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
				this->CurrentHeader->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
				this->MaxHeader->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
				this->UsePasswordHeader->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			}

			{
				this->SessionList->HideSelection = false;
				this->SessionList->Name = L"Session List";
				this->SessionList->Size = System::Drawing::Size(550, 300);
				this->SessionList->Location = System::Drawing::Point(25, 15);
				this->SessionList->UseCompatibleStateImageBehavior = false;
				this->SessionList->TabIndex = 0;
				this->SessionList->View = System::Windows::Forms::View::Details;
				this->SessionList->Columns->AddRange(gcnew array<System::Windows::Forms::ColumnHeader^>(5) {
					this->IDHeader, this->NameHeader, this->CurrentHeader, this->MaxHeader, this->UsePasswordHeader
				});
			}

			{
				this->JoinButton->BackColor = System::Drawing::SystemColors::ButtonHighlight;
				this->JoinButton->Font = gcnew System::Drawing::Font(L"NanumSquare ExtraBold", 16, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 129);
				this->JoinButton->Size = System::Drawing::Size(200, 50);
				this->JoinButton->Location = System::Drawing::Point((this->Size.Width / 2) - 100, this->SessionList->Size.Height + this->SessionList->Location.Y + 15);
				this->JoinButton->TabIndex = 2;
				this->JoinButton->Name = L"Join Button";
				this->JoinButton->Text = L"Join";
				this->JoinButton->UseVisualStyleBackColor = false;
				this->JoinButton->Click += gcnew System::EventHandler(this, &FindSessionForm::JoinButton_Click);
			}

			this->Controls->Add(this->SessionList);
			this->Controls->Add(this->JoinButton);
		}
	private:
		System::Void FindSessionForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void FindSessionForm_Closed(System::Object^ sender, System::EventArgs^ e);

	private:
		System::Void JoinButton_Click(System::Object^ sender, System::EventArgs^ e);

	private:
		System::Void PacketProcessing(array<unsigned char>^ Buffer);
		System::Void JoinSession_Delegate(array<unsigned char>^ Buffer);
		System::Void EnterNickname_Delegate(System::String^ nickname);

	};
}
