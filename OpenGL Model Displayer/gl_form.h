#pragma once

#include <windows.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "gl_robot.h"

#using <mscorlib.dll>

namespace OpenGLModelDisplayer {

  const std::string DEFAULT_VERTEX_SHADER_FILE_PATH = "..\\shader\\vertex_shader.glsl";
  const std::string DEFAULT_FRAGMENT_SHADER_FILE_PATH = "..\\shader\\fragment_shader.glsl";

  const std::string SHADER_ATTRIBUTE_VERTEX_POSITION_NAME = "vertex_position";
  const std::string SHADER_ATTRIBUTE_VERTEX_COLOR_NAME = "vertex_color";
  const std::string SHADER_ATTRIBUTE_VERTEX_NORMAL_NAME = "vertex_normal";
  const std::string SHADER_ATTRIBUTE_VERTEX_UV_NAME = "vertex_uv";

  const std::string SHADER_UNIFORM_modelview_matrix_NAME = "modelview_matrix";
  const std::string SHADER_UNIFORM_VIEW_MATRIX_NAME = "view_matrix";
  const std::string SHADER_UNIFORM_PROJECTION_MATRIX_NAME = "projection_matrix";
  const std::string SHADER_UNIFORM_INVERSE_modelview_matrix_NAME = "inverse_modelview_matrix";
  const std::string SHADER_UNIFORM_TRANSPOSE_INVERSE_modelview_matrix_NAME = "transpose_inverse_modelview_matrix";
  const std::string SHADER_UNIFORM_TEXTURE_NAME = "texture";

  const std::string DEFAULT_MODEL_OBJ_FILE_PATH = "..\\data\\Optimus prime\\optimus.obj";

  GLint shader_program_id;
  GLint shader_attribute_vertex_position_id;
  GLint shader_attribute_vertex_color_id;
  GLint shader_attribute_vertex_normal_id;
  GLint shader_attribute_vertex_uv_id;
  GLint shader_uniform_modelview_matrix_id;
  GLint shader_uniform_view_matrix_id;
  GLint shader_uniform_projection_matrix_id;
  GLint shader_uniform_inverse_modelview_matrix_id;
  GLint shader_uniform_transpose_inverse_modelview_matrix_id;
  GLint shader_uniform_texture_id;

  const int FPS = 120;
  const float FRAME_REFRESH_TIME = 1000.0f / FPS;

  glm::vec3 eye_position(2.0, 1.5, 2.0);
  glm::vec3 look_at_position(0.0, 0.0, 0.0);

  const float EYE_POSITION_SCALE_PER_SCROLLING = 0.9f;
  float eye_position_scale = 1.0;

  const float ROTATED_DEGREE_PER_SECOND = 30.0;
  float rotated_degree;

  GLRobot robot;

  using namespace System::Runtime::InteropServices;

  [DllImport("opengl32.dll")]
  extern HDC wglSwapBuffers(HDC hdc);

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Drawing;

  /// <summary>
  /// Summary for GLForm
  /// </summary>
  public ref class GLForm : public System::Windows::Forms::Form {

  public:
    GLForm(void) {
      InitializeComponent();

      srand((unsigned)time(0));
      InitializeOpenGL();

      this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelMouseWheel);
      this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelKeyDown);

      timer1->Interval = (int)FRAME_REFRESH_TIME;
      timer1->Enabled = true;

      timer1->Tick += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::Timer1Tick);
      timer1->Start();
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~GLForm() {
      if (components) {
        delete components;
      }
    }

  private:

    System::Windows::Forms::Panel^ gl_panel;
    System::Windows::Forms::Timer^ timer1;
    System::ComponentModel::IContainer^ components;

    static HWND hwnd;
    static HDC hdc;
    static HGLRC hrc;

    static bool ParseFileIntoString(const std::string &file_path, std::string &file_string) {
      std::ifstream file_stream(file_path);
      if (!file_stream.is_open()) {
        return false;
      }
      file_string = std::string(std::istreambuf_iterator<char>(file_stream), std::istreambuf_iterator<char>());
      return true;
    }

    void InitializeOpenGL() {

      // Get Handle
      hwnd = (HWND)this->gl_panel->Handle.ToInt32();
      hdc = GetDC(hwnd);
      wglSwapBuffers(hdc);

      // Set pixel format
      PIXELFORMATDESCRIPTOR pfd;
      pfd.nVersion = 1; 
      pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
      pfd.iPixelType = (byte)(PFD_TYPE_RGBA);
      pfd.cColorBits = 32;
      pfd.cDepthBits = 32;
      pfd.iLayerType = (byte)(PFD_MAIN_PLANE);

      SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

      // Create OpenGL Rendering Context
      hrc = (wglCreateContext(hdc));
      if (!hrc)  {
        std::cerr << "wglCreateContext failed.\n";
      }

      // Assign OpenGL Rendering Context
      if (!wglMakeCurrent(hdc, hrc)) {
        std::cerr << "wglMakeCurrent failed.\n";
      }

      if (glewInit() != GLEW_OK) {
        std::cerr << "OpenGL initialize failed.\n";
      }

      if (!GLEW_VERSION_2_0) {
        std::cerr << "Your graphic card does not support OpenGL 2.0.\n";
      }

      glewExperimental = GL_TRUE;

      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      std::string vertex_shader_string;
      std::string fragment_shader_string;

      ParseFileIntoString(DEFAULT_VERTEX_SHADER_FILE_PATH, vertex_shader_string);
      ParseFileIntoString(DEFAULT_FRAGMENT_SHADER_FILE_PATH, fragment_shader_string);

      GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      const GLchar *vertex_shader_pointer = (const GLchar*)vertex_shader_string.c_str();
      glShaderSource(vertex_shader, 1, &vertex_shader_pointer, NULL);
      glCompileShader(vertex_shader);

      int shader_compile_status;
      glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compile_status);
      if (shader_compile_status != GL_TRUE) {
        std::cerr << "Could not compile the shader " << DEFAULT_VERTEX_SHADER_FILE_PATH << " .\n";
      }

      GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      const GLchar *fragment_shader_pointer = (const GLchar*)fragment_shader_string.c_str();
      glShaderSource(fragment_shader, 1, &fragment_shader_pointer, NULL);
      glCompileShader(fragment_shader);

      glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_compile_status);
      if (shader_compile_status != GL_TRUE) {
        std::cerr << "Could not compile the shader " << DEFAULT_FRAGMENT_SHADER_FILE_PATH << " .\n";
      }

      shader_program_id = glCreateProgram();
      glAttachShader(shader_program_id, vertex_shader);
      glAttachShader(shader_program_id, fragment_shader);
      glLinkProgram(shader_program_id);

      int shader_link_status;
      glGetProgramiv(shader_program_id, GL_LINK_STATUS, &shader_link_status);
      if (shader_link_status != GL_TRUE) {
        std::cerr << "Could not link the shader.\n";
      }

      int shader_validate_status;
      glValidateProgram(shader_program_id);
      glGetProgramiv(shader_program_id, GL_VALIDATE_STATUS, &shader_validate_status);
      if (shader_validate_status != GL_TRUE) {
        std::cerr << "Could not validate the shader.\n";
      }

      shader_attribute_vertex_position_id = glGetAttribLocation(shader_program_id, SHADER_ATTRIBUTE_VERTEX_POSITION_NAME.c_str());
      if (shader_attribute_vertex_position_id == -1) {
        std::cerr << "Could not bind attribute " << SHADER_ATTRIBUTE_VERTEX_POSITION_NAME << ".\n";
      }

      shader_attribute_vertex_color_id = glGetAttribLocation(shader_program_id, SHADER_ATTRIBUTE_VERTEX_COLOR_NAME.c_str());
      if (shader_attribute_vertex_color_id == -1) {
        std::cerr << "Could not bind attribute " << SHADER_ATTRIBUTE_VERTEX_COLOR_NAME << ".\n";
      }

      shader_attribute_vertex_normal_id = glGetAttribLocation(shader_program_id, SHADER_ATTRIBUTE_VERTEX_NORMAL_NAME.c_str());
      if (shader_attribute_vertex_normal_id == -1) {
        std::cerr << "Could not bind attribute " << SHADER_ATTRIBUTE_VERTEX_NORMAL_NAME << ".\n";
      }

      shader_attribute_vertex_uv_id = glGetAttribLocation(shader_program_id, SHADER_ATTRIBUTE_VERTEX_UV_NAME.c_str());
      if (shader_attribute_vertex_uv_id == -1) {
        std::cerr << "Could not bind attribute " << SHADER_ATTRIBUTE_VERTEX_UV_NAME << ".\n";
      }

      shader_uniform_modelview_matrix_id = glGetUniformLocation(shader_program_id, SHADER_UNIFORM_modelview_matrix_NAME.c_str());
      if (shader_uniform_modelview_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_UNIFORM_modelview_matrix_NAME << ".\n";
      }

      shader_uniform_view_matrix_id = glGetUniformLocation(shader_program_id, SHADER_UNIFORM_VIEW_MATRIX_NAME.c_str());
      if (shader_uniform_view_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_UNIFORM_VIEW_MATRIX_NAME << ".\n";
      }

      shader_uniform_projection_matrix_id = glGetUniformLocation(shader_program_id, SHADER_UNIFORM_PROJECTION_MATRIX_NAME.c_str());
      if (shader_uniform_projection_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_UNIFORM_PROJECTION_MATRIX_NAME << ".\n";
      }

      shader_uniform_inverse_modelview_matrix_id = glGetUniformLocation(shader_program_id, SHADER_UNIFORM_INVERSE_modelview_matrix_NAME.c_str());
      if (shader_uniform_inverse_modelview_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_UNIFORM_INVERSE_modelview_matrix_NAME << ".\n";
      }

      shader_uniform_transpose_inverse_modelview_matrix_id = glGetUniformLocation(shader_program_id, SHADER_UNIFORM_TRANSPOSE_INVERSE_modelview_matrix_NAME.c_str());
      if (shader_uniform_transpose_inverse_modelview_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_UNIFORM_TRANSPOSE_INVERSE_modelview_matrix_NAME << ".\n";
      }

      shader_uniform_texture_id = glGetUniformLocation(shader_program_id, SHADER_UNIFORM_TEXTURE_NAME.c_str());
      if (shader_uniform_texture_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_UNIFORM_TEXTURE_NAME << ".\n";
      }

      if (!robot.BuildMeshFromObjFile(DEFAULT_MODEL_OBJ_FILE_PATH)) {
        robot.BuildSimpleMesh();
      }

      robot.WalkingMode();
    }

    void RenderGLPanel() {      
      wglMakeCurrent(hdc, hrc);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      float aspect_ratio = this->gl_panel->Size.Width / (float)this->gl_panel->Size.Height;

      glm::mat4 projection_matrix = glm::perspective(45.0f, aspect_ratio, 0.1f, 100.0f);

      glm::mat4 view_matrix = glm::lookAt(eye_position * eye_position_scale, look_at_position, glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 modelview_matrix = glm::mat4(1.0f);
      modelview_matrix = glm::rotate(modelview_matrix, glm::radians(rotated_degree), glm::vec3(0, 1, 0));

      glUseProgram(shader_program_id);

      glUniformMatrix4fv(shader_uniform_projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
      glUniformMatrix4fv(shader_uniform_view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));

      robot.Draw(modelview_matrix);

      SwapBuffers(hdc);
    }

    System::Void Timer1Tick(System::Object^ sender, System::EventArgs^ e) {
      rotated_degree = rotated_degree + ROTATED_DEGREE_PER_SECOND * (FRAME_REFRESH_TIME / 1000.0f);
      rotated_degree = rotated_degree - ((rotated_degree > 360) ? 360 : 0);
      rotated_degree = 0;

      RenderGLPanel();

      robot.Update(FRAME_REFRESH_TIME);
    }

    System::Void GLPanelKeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
      switch (e->KeyCode) {
      case Keys::F1:
        break;
      case Keys::W:
      case Keys::Up:
        eye_position.y += 1;
        look_at_position.y += 1;
        break;
      case Keys::A:
      case Keys::Left:
        eye_position.x -= 1;
        look_at_position.x -= 1;
        break;
      case Keys::S:
      case Keys::Down:
        eye_position.y -= 1;
        look_at_position.y -= 1;
        break;
      case Keys::D:
      case Keys::Right:
        eye_position.x += 1;
        look_at_position.x += 1;
        break;
      }
    }

    System::Void GLPanelMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
      eye_position_scale *= (e->Delta < 0) ? (1.0f / EYE_POSITION_SCALE_PER_SCROLLING) : EYE_POSITION_SCALE_PER_SCROLLING;
    }

    /// <summary>
    /// Required designer variable.
    /// </summary>


#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void) {
      this->components = (gcnew System::ComponentModel::Container());
      this->gl_panel = (gcnew System::Windows::Forms::Panel());
      this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
      this->SuspendLayout();
      // 
      // gl_panel
      // 
      this->gl_panel->Location = System::Drawing::Point(12, 12);
      this->gl_panel->Name = L"gl_panel";
      this->gl_panel->Size = System::Drawing::Size(800, 600);
      this->gl_panel->TabIndex = 0;
      // 
      // GLForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(1008, 730);
      this->Controls->Add(this->gl_panel);
      this->Name = L"GLForm";
      this->Text = L"GLForm";
      this->Load += gcnew System::EventHandler(this, &GLForm::GLForm_Load);
      this->ResumeLayout(false);
    }
#pragma endregion
  private:

    System::Void GLForm_Load(System::Object^  sender, System::EventArgs^ e) {}
  };
}
