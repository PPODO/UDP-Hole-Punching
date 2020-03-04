#pragma once

namespace HolepunchingClientchatroom {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class FindSessionForm : public System::Windows::Forms::Form
	{
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
		System::ComponentModel::Container ^components;

	private:
		System::Windows::Forms::ListView^ SessionList;

#pragma region Windows Form Designer generated code
		void InitializeComponent(System::Windows::Forms::Form^ Owner) {
			SessionList = gcnew System::Windows::Forms::ListView();

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

		}
#pragma endregion
	private: 
		System::Void FindSessionForm_Load(System::Object^ sender, System::EventArgs^ e) {
			if (this->Owner->Validate()) {
				this->Owner->Enabled = false;
			}
		}

		System::Void FindSessionForm_Closed(System::Object^ sender, System::EventArgs^ e) {
			if (this->Owner->Validate()) {
				this->Owner->Enabled = true;
			}
		}
	};
}
