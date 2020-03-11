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
		System::Windows::Forms::ColumnHeader^ NameHeader;
		System::Windows::Forms::ColumnHeader^ CurrentHeader;
		System::Windows::Forms::ColumnHeader^ MaxHeader;
		System::Windows::Forms::ColumnHeader^ UsePasswordHeader;

	private:

		void InitializeComponent(System::Windows::Forms::Form^ Owner) {
			SessionList = gcnew System::Windows::Forms::ListView();
			NameHeader = gcnew System::Windows::Forms::ColumnHeader();
			CurrentHeader = gcnew System::Windows::Forms::ColumnHeader();
			MaxHeader = gcnew System::Windows::Forms::ColumnHeader();
			UsePasswordHeader = gcnew System::Windows::Forms::ColumnHeader();

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
				this->NameHeader->Text = L"Name";
				this->CurrentHeader->Text = L"Current";
				this->MaxHeader->Text = L"Max";
				this->UsePasswordHeader->Text = L"Use Password";

				this->NameHeader->Width = 550 / 4;
				this->CurrentHeader->Width = 550 / 4;
				this->MaxHeader->Width = 550 / 4;
				this->UsePasswordHeader->Width = 550 / 4;

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
				this->SessionList->Columns->AddRange(gcnew array<System::Windows::Forms::ColumnHeader^>(4) {
					this->NameHeader, this->CurrentHeader, this->MaxHeader, this->UsePasswordHeader
				});
			}

			this->Controls->Add(this->SessionList);
		}
	private:
		System::Void FindSessionForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void FindSessionForm_Closed(System::Object^ sender, System::EventArgs^ e);

	private:
		System::Void PacketProcessing(const Packets::Types::CFindPacket& Packet);

	};
}
