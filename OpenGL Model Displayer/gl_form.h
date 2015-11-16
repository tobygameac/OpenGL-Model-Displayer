#pragma once

#include <windows.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <chrono>
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
#include "gl_simple_model.h"

#include <msclr\marshal_cppstd.h>
#using <mscorlib.dll>

namespace OpenGLModelDisplayer {

  enum ProgramStatus {
    VIEW_OBJ_FILE,
    VIEW_ROBOT_ANIMATION
  };

  ProgramStatus program_status = VIEW_OBJ_FILE;

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

  const int FPS = 60;
  const float FRAME_REFRESH_TIME = 1000.0f / FPS;

  glm::vec3 eye_position(0, 1, 1);
  glm::vec3 look_at_position(0.0, 0.0, 0.0);

  const float EYE_POSITION_SCALE_PER_SCROLLING = 0.9f;
  float eye_position_scale = 1.0;

  const glm::vec3 ROTATED_DEGREE_PER_SECOND(0.0, 360.0 / 8.0, 0.0);
  glm::vec3 rotated_degree;

  GLSimpleModel obj_model;
  GLRobot robot;

  int mouse_last_x;
  int mouse_last_y;

  std::chrono::steady_clock::time_point last_clock = std::chrono::steady_clock::now();

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

      this->open_obj_tool_strip_menu_item->Click += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::OpenObjButtonClick);

      this->hint_label->Text = "Press mouse : Adjust view angle \n Scroll mouse : Change eyes distance \n F1 : Stand \n F2 : Walk \n F3 : Run \n F4 : Kungfu \n F5 : Spike";

      this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelMouseWheel);
      this->gl_panel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelMouseMove);
      this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelKeyDown);

      this->frame_refresh_timer->Interval = (int)FRAME_REFRESH_TIME;
      this->frame_refresh_timer->Enabled = true;

      this->frame_refresh_timer->Tick += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::FrameRefreshTimerTick);
      this->frame_refresh_timer->Start();
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

    System::Windows::Forms::Panel ^gl_panel;
    System::Windows::Forms::Label ^hint_label;
    System::Windows::Forms::Timer ^frame_refresh_timer;
    System::ComponentModel::IContainer ^components;
    System::Windows::Forms::MenuStrip ^file_menu_strip;
    System::Windows::Forms::ToolStripMenuItem ^file_tool_strip_menu_item;
    System::Windows::Forms::ToolStripMenuItem ^open_obj_tool_strip_menu_item;

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
      if (!hrc) {
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

      if (!robot.BuildMeshFromOptimusPrimeObjFile(DEFAULT_MODEL_OBJ_FILE_PATH)) {
        robot.BuildSimpleMesh();
      }

      if (!obj_model.BuildMeshFromObjFile(DEFAULT_MODEL_OBJ_FILE_PATH)) {
      }
    }

    void RenderGLPanel() {
      wglMakeCurrent(hdc, hrc);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      float aspect_ratio = this->gl_panel->Size.Width / (float)this->gl_panel->Size.Height;

      glm::mat4 projection_matrix = glm::perspective(45.0f, aspect_ratio, 0.01f, 1000.0f);

      glm::mat4 view_matrix = glm::lookAt(eye_position * eye_position_scale, look_at_position, glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 modelview_matrix = glm::mat4(1.0f);

      view_matrix = glm::rotate(view_matrix, glm::radians(rotated_degree.x), glm::vec3(1, 0, 0));
      view_matrix = glm::rotate(view_matrix, glm::radians(rotated_degree.y), glm::vec3(0, 1, 0));
      view_matrix = glm::rotate(view_matrix, glm::radians(rotated_degree.z), glm::vec3(0, 0, 1));

      glUseProgram(shader_program_id);

      glUniformMatrix4fv(shader_uniform_projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
      glUniformMatrix4fv(shader_uniform_view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));

      if (program_status == VIEW_OBJ_FILE) {
        obj_model.Draw(modelview_matrix);
      } else if (program_status == VIEW_ROBOT_ANIMATION) {
        robot.Draw(modelview_matrix);
      }

      SwapBuffers(hdc);
    }

    void OpenObjButtonClick(System::Object ^sender, System::EventArgs ^e) {
      OpenFileDialog ^open_obj_file_dialog = gcnew OpenFileDialog();
      open_obj_file_dialog->Filter = "Obj files | *.obj";
      open_obj_file_dialog->Title = "Open a obj file.";

      if (open_obj_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        obj_model.BuildMeshFromObjFile(msclr::interop::marshal_as<std::string>(open_obj_file_dialog->FileName));
      }
    }


    System::Void FrameRefreshTimerTick(System::Object ^sender, System::EventArgs ^e) {
      RenderGLPanel();

      std::chrono::steady_clock::time_point current_clock = std::chrono::steady_clock::now();
      float elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_clock - last_clock).count();
      last_clock = current_clock;

      if (program_status == VIEW_OBJ_FILE) {
        rotated_degree = rotated_degree + ROTATED_DEGREE_PER_SECOND * (FRAME_REFRESH_TIME / 1000.0f);
        obj_model.Update(elapsed_time);
      } else if (program_status == VIEW_ROBOT_ANIMATION) {
        robot.Update(elapsed_time);
      }
    }

    System::Void GLPanelKeyDown(System::Object ^sender, System::Windows::Forms::KeyEventArgs ^e) {
      if (program_status == VIEW_OBJ_FILE) {
      } else if (program_status == VIEW_ROBOT_ANIMATION) {
        switch (e->KeyCode) {
        case Keys::F1:
          robot.ClearAnimation();
          break;
        case Keys::F2:
          robot.WalkingMode();
          break;
        case Keys::F3:
          robot.RunningMode();
          break;
        case Keys::F4:
          robot.KongfuMode();
          break;
        case Keys::F5:
          robot.SpikeMode();
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
    }

    System::Void GLPanelMouseWheel(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) {
      eye_position_scale *= (e->Delta < 0) ? (1.0f / EYE_POSITION_SCALE_PER_SCROLLING) : EYE_POSITION_SCALE_PER_SCROLLING;
    }

    System::Void GLPanelMouseMove(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e) {
      if (e->Button == System::Windows::Forms::MouseButtons::Left || e->Button == System::Windows::Forms::MouseButtons::Right) {
        rotated_degree.y += (e->X - mouse_last_x);
        rotated_degree.x += (e->Y - mouse_last_y);
      }
      mouse_last_x = e->X;
      mouse_last_y = e->Y;
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
      this->frame_refresh_timer = (gcnew System::Windows::Forms::Timer(this->components));
      this->hint_label = (gcnew System::Windows::Forms::Label());
      this->file_menu_strip = (gcnew System::Windows::Forms::MenuStrip());
      this->file_tool_strip_menu_item = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->open_obj_tool_strip_menu_item = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->file_menu_strip->SuspendLayout();
      this->SuspendLayout();
      // 
      // gl_panel
      // 
      this->gl_panel->Location = System::Drawing::Point(50, 50);
      this->gl_panel->Name = L"gl_panel";
      this->gl_panel->Size = System::Drawing::Size(800, 600);
      this->gl_panel->TabIndex = 0;
      // 
      // hint_label
      // 
      this->hint_label->AutoSize = true;
      this->hint_label->Location = System::Drawing::Point(900, 50);
      this->hint_label->Name = L"hint_label";
      this->hint_label->Size = System::Drawing::Size(52, 13);
      this->hint_label->TabIndex = 1;
      this->hint_label->Text = L"hint_label";
      // 
      // file_menu_strip
      // 
      this->file_menu_strip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^>(1) {
        this->file_tool_strip_menu_item
      });
      this->file_menu_strip->Location = System::Drawing::Point(0, 0);
      this->file_menu_strip->Name = L"file_menu_strip";
      this->file_menu_strip->Size = System::Drawing::Size(1008, 24);
      this->file_menu_strip->TabIndex = 2;
      this->file_menu_strip->Text = L"menuStrip1";
      // 
      // file_tool_strip_menu_item
      // 
      this->file_tool_strip_menu_item->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem ^>(1) {
        this->open_obj_tool_strip_menu_item
      });
      this->file_tool_strip_menu_item->Name = L"file_tool_strip_menu_item";
      this->file_tool_strip_menu_item->Size = System::Drawing::Size(37, 20);
      this->file_tool_strip_menu_item->Text = L"File";
      // 
      // open_obj_tool_strip_menu_item
      // 
      this->open_obj_tool_strip_menu_item->Name = L"open_obj_tool_strip_menu_item";
      this->open_obj_tool_strip_menu_item->Size = System::Drawing::Size(152, 22);
      this->open_obj_tool_strip_menu_item->Text = L"Open obj";
      // 
      // GLForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(1008, 730);
      this->Controls->Add(this->hint_label);
      this->Controls->Add(this->gl_panel);
      this->Controls->Add(this->file_menu_strip);
      this->MainMenuStrip = this->file_menu_strip;
      this->Name = L"GLForm";
      this->Text = L"GLForm";
      this->Load += gcnew System::EventHandler(this, &GLForm::GLForm_Load);
      this->file_menu_strip->ResumeLayout(false);
      this->file_menu_strip->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  private:

    System::Void GLForm_Load(System::Object ^sender, System::EventArgs ^e) {
    }
  };
}
