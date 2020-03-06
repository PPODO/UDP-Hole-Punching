#pragma once
#include "FindSessionForm.h"
#include "CreateSessionForm.h"
#include "SocketClass.h"
#include "Packets.h"
#include <thread>

namespace HolepunchingClientchatroom {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void) : m_UdpSocket(gcnew AsyncSocket::UdpAsyncObject(gcnew IPEndPoint(IPAddress::Parse("59.5.200.189"), 3550), gcnew System::AsyncCallback(this, &MainForm::HandleDataReceive))) {
			InitializeComponent();

		}

	protected:
		~MainForm() {
			if (components) {
				delete components;
			}
		}
	private: 
		System::Windows::Forms::Label^ Title;
		System::Windows::Forms::Button^ Find;
		System::Windows::Forms::Button^ Create;

	private:
		System::ComponentModel::Container^ components;

	private:
		AsyncSocket::UdpAsyncObject^ m_UdpSocket;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void) {
			this->Title = (gcnew System::Windows::Forms::Label());
			this->Find = (gcnew System::Windows::Forms::Button());
			this->Create = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// Title
			// 
			{
				this->Title->AutoSize = true;
				this->Title->Font = (gcnew System::Drawing::Font(L"NanumSquare ExtraBold", 36, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(129)));
				this->Title->Location = System::Drawing::Point(320, 40);
				this->Title->Name = L"Title";
				this->Title->Size = System::Drawing::Size(441, 53);
				this->Title->TabIndex = 0;
				this->Title->Text = L"Chatting Program";
			}
			// 
			// Find
			// 
			{
				this->Find->BackColor = System::Drawing::SystemColors::ButtonHighlight;
				this->Find->Font = (gcnew System::Drawing::Font(L"NanumSquare ExtraBold", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(129)));
				this->Find->Location = System::Drawing::Point(403, 275);
				this->Find->Name = L"Find";
				this->Find->Size = System::Drawing::Size(275, 75);
				this->Find->TabIndex = 1;
				this->Find->Text = L"Find Session";
				this->Find->UseVisualStyleBackColor = false;
				this->Find->Click += gcnew System::EventHandler(this, &MainForm::Find_Click);
			}
			// 
			// Create
			//
			{
				this->Create->BackColor = System::Drawing::SystemColors::ButtonHighlight;
				this->Create->Font = (gcnew System::Drawing::Font(L"NanumSquare ExtraBold", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(129)));
				this->Create->Location = System::Drawing::Point(403, 425);
				this->Create->Name = L"Create";
				this->Create->Size = System::Drawing::Size(275, 75);
				this->Create->TabIndex = 2;
				this->Create->Text = L"Create Session";
				this->Create->UseVisualStyleBackColor = false;
				this->Create->Click += gcnew System::EventHandler(this, &MainForm::Create_Click);
			}
			// 
			// MainForm
			// 
			{
				this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->BackColor = System::Drawing::SystemColors::AppWorkspace;
				this->ClientSize = System::Drawing::Size(1080, 576);
				this->Controls->Add(this->Create);
				this->Controls->Add(this->Find);
				this->Controls->Add(this->Title);
				this->MaximizeBox = false;
				this->Name = L"MainForm";
				this->Text = L"Chat Program";
				this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
				this->ResumeLayout(false);
				this->PerformLayout();
			}
		}
#pragma endregion
	// Load
	private: 
		System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {
			std::stringstream Stream;
			Stream << Packets::Types::CPacket(Packets::MessageType::EMESSAGETYPE::EMT_CONNECT);

			AsyncSocket::SendTo(m_UdpSocket, gcnew String(Stream.str().c_str()));
		}

	// Click Event
	private:
		System::Void Find_Click(System::Object^ sender, System::EventArgs^ e) {
			FindSessionForm^ ModalObject = gcnew FindSessionForm{ this };
			ModalObject->Show();
		}

		System::Void Create_Click(System::Object^ sender, System::EventArgs^ e) {
			CreateSessionForm^ ModalObject = gcnew CreateSessionForm{this};
			ModalObject->Show();
		}

	//
	private:
		System::Void HandleDataReceive(System::IAsyncResult^ Result) {
			auto SocketObj = safe_cast<AsyncSocket::UdpAsyncObject^>(Result->AsyncState);
			int ReceiveBytes = 0;

			if (SocketObj && (ReceiveBytes = SocketObj->m_Socket->EndReceiveFrom(Result, reinterpret_cast<EndPoint^%>(SocketObj->m_RemoteAddress))) > 0) {
				pin_ptr<unsigned char> Pointer = &SocketObj->m_ReceiveBuffer[0];
				std::cout << "Rece Data : " << &(*Pointer) << std::endl;
			}
			else {
				Console::WriteLine("Receive Failure!");
			}
			AsyncSocket::ReceiveFrom(m_UdpSocket);
		}

	};
}
