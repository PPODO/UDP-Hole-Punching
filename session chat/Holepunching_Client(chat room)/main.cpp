#include <iostream>
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	HolepunchingClientchatroom::MainForm MainFormObject;
	Application::Run(%MainFormObject);

	return 0;
}