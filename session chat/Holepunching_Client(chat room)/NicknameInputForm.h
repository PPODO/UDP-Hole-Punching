#pragma once

namespace HolepunchingClientchatroom {
	delegate void EnterNicknameDelegate(System::String^ Nickname);

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class NicknameInputForm : public System::Windows::Forms::Form {
	public:
		NicknameInputForm(System::Windows::Forms::Form^ Owner, EnterNicknameDelegate^ Delegate) : m_EnterNicknameDelegate(Delegate) {
			InitializeComponent(Owner);
		}

	protected:
		~NicknameInputForm() {
			if (components) {
				delete components;
			}
		}

	private:
		System::ComponentModel::Container^ components;

	private:
		System::Windows::Forms::Label^ m_Label;
		System::Windows::Forms::TextBox^ m_InputBox;
		System::Windows::Forms::Button^ m_ConfirmButton;

	private:
		EnterNicknameDelegate^ m_EnterNicknameDelegate;

	private:
		void InitializeComponent(System::Windows::Forms::Form^ Owner) {
			this->m_Label = gcnew System::Windows::Forms::Label();
			this->m_InputBox = gcnew System::Windows::Forms::TextBox();
			this->m_ConfirmButton = gcnew System::Windows::Forms::Button();

			{
				this->m_Label->Font = gcnew System::Drawing::Font("NanumSquare ExtraBold", 15, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 100);
				this->m_Label->Name = L"NickNameLabel";
				this->m_Label->Text = L"Nickname";
				this->m_Label->Size = System::Drawing::Size(200, 25);
				this->m_Label->Location = System::Drawing::Point(100, 25);
				this->m_Label->TabIndex = 0;

				this->m_InputBox->Size = System::Drawing::Size(150, 0);
				this->m_InputBox->Location = System::Drawing::Point(75, 100);
				this->m_InputBox->Name = L"Nickname Input Box";
				this->m_InputBox->TabIndex = 2;
				this->m_InputBox->MaxLength = 15;

				this->m_ConfirmButton->BackColor = System::Drawing::SystemColors::ButtonHighlight;
				this->m_ConfirmButton->Font = gcnew System::Drawing::Font("NanumSquare ExtraBold", 15, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 100);
				this->m_ConfirmButton->Name = L"ConfirmButton";
				this->m_ConfirmButton->Text = L"Confirm";
				this->m_ConfirmButton->Size = System::Drawing::Size(100, 50);
				this->m_ConfirmButton->Location = System::Drawing::Point(100, 175);
				this->m_ConfirmButton->TabIndex = 1;
				this->m_ConfirmButton->UseVisualStyleBackColor = false;
				this->m_ConfirmButton->Click += gcnew System::EventHandler(this, &NicknameInputForm::ConfirmButton_Click);
			}

			this->Owner = Owner;
			this->SuspendLayout();
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(300, 250);
			this->Name = L"NicknameInputForm";
			this->Text = L"Set Your Nickname";
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->Location = System::Drawing::Point(Owner->Location.X + (Owner->Size.Width / 2) - (this->Size.Width / 2), Owner->Location.Y + (Owner->Size.Height / 2) - (this->Size.Height / 2));
			this->Load += gcnew System::EventHandler(this, &NicknameInputForm::NicknameInputForm_Load);
			this->Controls->Add(this->m_Label);
			this->Controls->Add(this->m_InputBox);
			this->Controls->Add(this->m_ConfirmButton);
			this->ResumeLayout(false);

		}

	private:
		System::Void NicknameInputForm_Load(System::Object^ sender, System::EventArgs^ e) {
		}

		System::Void ConfirmButton_Click(System::Object^ sender, System::EventArgs^ e) {
			if (this->m_InputBox->TextLength <= 0) {
				System::Windows::Forms::MessageBox::Show("Nickname Must Be At Least 0 Characters Long!", "Warning", System::Windows::Forms::MessageBoxButtons::OK);
			}
			else if (this->m_EnterNicknameDelegate != nullptr) {
				this->m_EnterNicknameDelegate(this->m_InputBox->Text);
				this->Close();
			}
		}
	};
}
