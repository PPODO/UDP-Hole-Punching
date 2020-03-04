#pragma once

namespace HolepunchingClientchatroom {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class CreateSessionForm : public System::Windows::Forms::Form
	{
	public:
		CreateSessionForm(System::Windows::Forms::Form^ _Owner) {
			InitializeComponent(_Owner);
		}

	protected:
		~CreateSessionForm() {
			if (components) {
				delete components;
			}
		}

	private:
		System::ComponentModel::Container ^components;

		System::Windows::Forms::Label^ SessionNameLabel;
		System::Windows::Forms::Label^ MaxCountLabel;
		System::Windows::Forms::Label^ UsePasswordLabel;
		System::Windows::Forms::Label^ PasswordLabel;

		System::Windows::Forms::TextBox^ SessionNameTextBox;
		System::Windows::Forms::ComboBox^ MaxCountComboBox;
		System::Windows::Forms::CheckBox^ UsePasswordCheckBox;
		System::Windows::Forms::TextBox^ PasswordTextBox;
		System::Windows::Forms::Button^ ConfirmButton;

#pragma region Windows Form Designer generated code
		void InitializeComponent(System::Windows::Forms::Form^ _Owner) {
			ConfirmButton = gcnew System::Windows::Forms::Button();
			SessionNameTextBox = gcnew System::Windows::Forms::TextBox();
			MaxCountComboBox = gcnew System::Windows::Forms::ComboBox();
			PasswordTextBox = gcnew System::Windows::Forms::TextBox();
			UsePasswordCheckBox = gcnew System::Windows::Forms::CheckBox();

			SessionNameLabel = gcnew System::Windows::Forms::Label();
			MaxCountLabel = gcnew System::Windows::Forms::Label();
			UsePasswordLabel = gcnew System::Windows::Forms::Label();
			PasswordLabel = gcnew System::Windows::Forms::Label();

			this->Owner = _Owner;
			this->SuspendLayout();
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(600, 400);
			this->StartPosition = FormStartPosition::Manual;
			this->Location = System::Drawing::Point(_Owner->Location.X + (_Owner->ClientSize.Width / 2) - (this->ClientSize.Width / 2), _Owner->Location.Y + (_Owner->ClientSize.Height / 2) - (this->ClientSize.Height / 2));
			this->Name = L"CreateSessionForm";
			this->Text = L"Create Session";
			this->MaximizeBox = false;
			this->Load += gcnew System::EventHandler(this, &CreateSessionForm::CreateSessionForm_Load);
			this->Closed += gcnew System::EventHandler(this, &CreateSessionForm::CreateSessionForm_Closed);
			this->ResumeLayout(false);

			this->ConfirmButton->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->ConfirmButton->Font = (gcnew System::Drawing::Font(L"NanumSquare ExtraBold", 16, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(129)));
			this->ConfirmButton->Location = System::Drawing::Point(300 - (125 / 2), 325);
			this->ConfirmButton->Name = "ConfirmButton";
			this->ConfirmButton->Size = System::Drawing::Size(125, 35);
			this->ConfirmButton->TabIndex = 2;
			this->ConfirmButton->Text = "Confirm";
			this->ConfirmButton->UseVisualStyleBackColor = false;
			this->ConfirmButton->Click += gcnew System::EventHandler(this, &CreateSessionForm::ConfirmButton_Click);

			// Session Name
			{
				this->SessionNameTextBox->Size = System::Drawing::Size(250, 25);
				this->SessionNameTextBox->Location = System::Drawing::Point(300 - (this->SessionNameTextBox->Size.Width / 2), 100);
				this->SessionNameTextBox->Name = "Session Name";
				this->SessionNameTextBox->TabIndex = 3;

				this->SessionNameLabel->AutoSize = true;
				this->SessionNameLabel->Font = (gcnew System::Drawing::Font(L"NanumBarunGothic", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(129)));
				this->SessionNameLabel->Name = "SessionNameLabel";
				this->SessionNameLabel->Text = "Session Name :";
				this->SessionNameLabel->Location = System::Drawing::Point(300 - this->SessionNameTextBox->Size.Width - 5, 102);
				this->SessionNameLabel->Size = System::Drawing::Size(50, 25);
				this->SessionNameLabel->TabIndex = 0;
			}

			// Max Count
			{
				this->MaxCountComboBox->FormattingEnabled = true;
				this->MaxCountComboBox->Size = System::Drawing::Size(125, 20);
				this->MaxCountComboBox->Location = System::Drawing::Point(300 - 125, 150);
				this->MaxCountComboBox->Name = "MaxCount";
				this->MaxCountComboBox->TabIndex = 3;
				for (int i = 0; i < 10; i++) {
					this->MaxCountComboBox->Items->Add(i + 1);
				}
				this->MaxCountComboBox->SelectedIndex = 0;

				this->MaxCountLabel->AutoSize = true;
				this->MaxCountLabel->Font = (gcnew System::Drawing::Font(L"NanumBarunGothic", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(129)));
				this->MaxCountLabel->Name = "MaxCountLabel";
				this->MaxCountLabel->Text = "Max Count :";
				this->MaxCountLabel->Location = System::Drawing::Point(300 - (this->MaxCountComboBox->Size.Width * 2) + 20, 152);
				this->MaxCountLabel->Size = System::Drawing::Size(50, 25);
				this->MaxCountLabel->TabIndex = 0;
			}

			// Use Password
			{
				this->UsePasswordCheckBox->AutoSize = true;
				this->UsePasswordCheckBox->Size = System::Drawing::Size(86, 16);
				this->UsePasswordCheckBox->Location = System::Drawing::Point(300 - (this->UsePasswordCheckBox->Size.Width + 39), 202);
				this->UsePasswordCheckBox->TabIndex = 3;
				this->UsePasswordCheckBox->Name = "UsePassword";
				this->UsePasswordCheckBox->Text = "Use Password";
				this->UsePasswordCheckBox->UseVisualStyleBackColor = false;
				this->UsePasswordCheckBox->CheckedChanged += gcnew System::EventHandler(this, &CreateSessionForm::UsePasswordCheckBox_CheckedChanged);

				this->UsePasswordLabel->AutoSize = true;
				this->UsePasswordLabel->Font = (gcnew System::Drawing::Font(L"NanumBarunGothic", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(129)));
				this->UsePasswordLabel->Name = "UsePasswordLabel";
				this->UsePasswordLabel->Text = "Use Password :";
				this->UsePasswordLabel->Location = System::Drawing::Point(300 - this->UsePasswordCheckBox->Size.Width - 167, 202);
				this->UsePasswordLabel->Size = System::Drawing::Size(50, 25);
				this->UsePasswordLabel->TabIndex = 0;
			}

			// Input Password
			{
				this->PasswordTextBox->Size = System::Drawing::Size(250, 25);
				this->PasswordTextBox->Location = System::Drawing::Point(300 - (this->PasswordTextBox->Size.Width / 2), 250);
				this->PasswordTextBox->Name = "PasswordInput";
				this->PasswordTextBox->TabIndex = 3;
				this->PasswordTextBox->Enabled = false;

				this->PasswordLabel->AutoSize = true;
				this->PasswordLabel->Font = (gcnew System::Drawing::Font(L"NanumBarunGothic", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(129)));
				this->PasswordLabel->Name = "PasswordLabel";
				this->PasswordLabel->Text = "Password :";
				this->PasswordLabel->Location = System::Drawing::Point(300 - this->PasswordTextBox->Size.Width + 30, 252);
				this->PasswordLabel->Size = System::Drawing::Size(50, 25);
				this->PasswordLabel->TabIndex = 0;
			}

			this->Controls->Add(this->ConfirmButton);

			this->Controls->Add(this->SessionNameLabel);
			this->Controls->Add(this->SessionNameTextBox);

			this->Controls->Add(this->MaxCountComboBox);
			this->Controls->Add(this->MaxCountLabel);

			this->Controls->Add(this->UsePasswordCheckBox);
			this->Controls->Add(this->UsePasswordLabel);

			this->Controls->Add(this->PasswordTextBox);
			this->Controls->Add(this->PasswordLabel);
		}
#pragma endregion
	private: 
		System::Void CreateSessionForm_Load(System::Object^ sender, System::EventArgs^ e) {
			if (this->Owner->Validate()) {
				this->Owner->Enabled = false;
			}
		}

		System::Void CreateSessionForm_Closed(System::Object^ sender, System::EventArgs^ e) {
			if (this->Owner->Validate()) {
				this->Owner->Enabled = true;
			}
		}

	private:
		System::Void UsePasswordCheckBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
			if (this->UsePasswordCheckBox->Checked) {
				this->PasswordTextBox->Enabled = true;
			}
			else {
				this->PasswordTextBox->Enabled = false;
			}
		}

	private:
		System::Void ConfirmButton_Click(System::Object^ sender, System::EventArgs^ e) {

		}
};
}
