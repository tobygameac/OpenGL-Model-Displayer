#include "gl_form.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^> ^args) {
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false);
  OpenGLModelDisplayer::GLForm form;
  Application::Run(%form);
  return 0;
}