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
    VIEW_BASIC_DEMO,
    VIEW_OBJ,
    VIEW_ROBOT
  };

  ProgramStatus program_status;

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
  const std::string SHADER_UNIFORM_TEXTURE_FLAG_NAME = "texture_flag";

  const std::string ROBOT_MODEL_OBJ_FILE_PATH = "..\\data\\Optimus prime\\optimus.obj";

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
  GLint shader_uniform_texture_flag_id;

  const int FPS = 60;
  const float REFRESH_TIME_SCALE_FOR_NOT_ACCURATE_INTERVEL = 10.0;
  const float FRAME_REFRESH_TIME = (1000.0 / FPS) / REFRESH_TIME_SCALE_FOR_NOT_ACCURATE_INTERVEL;

  const glm::vec3 DEFAULT_EYE_POSITION(3, 3, 3);
  glm::vec3 eye_position = DEFAULT_EYE_POSITION;
  glm::vec3 look_at_position(0.0, 0.0, 0.0);

  const float EYE_POSITION_SCALE_PER_SCROLLING = 0.9f;
  float eye_position_scale = 1.0;

  const glm::vec3 ROTATED_DEGREE_PER_SECOND(0.0, 360.0 / 8.0, 0.0);
  glm::vec3 rotated_degree;

  bool rotate_view_matrix = true;

  GLSimpleModel basic_demo_model;
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

      basic_demo_model.BuildBasicDemoShape();

      if (!robot.BuildMeshFromOptimusPrimeObjFile(ROBOT_MODEL_OBJ_FILE_PATH)) {
        robot.BuildSimpleMesh();
      }

      ChangeProgramStatus(VIEW_BASIC_DEMO);

      open_obj_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::OpenObjButtonClick);
      view_basic_demo_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::ViewBasicDemoButtonClick);
      view_obj_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::ViewObjButtonClick);
      view_robot_tool_strip_menu_item_->Click += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::ViewRobotButtonClick);

      button_switch_rotate_mode_->Click += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::SwitchRotateModeButtonClick);
      button_reset_eye_->Click += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::ResetEyeButtonClick);

      gl_panel_->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelMouseMove);
      this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelMouseWheel);
      this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &OpenGLModelDisplayer::GLForm::GLPanelKeyDown);

      frame_refresh_timer_->Interval = (int)FRAME_REFRESH_TIME;
      frame_refresh_timer_->Enabled = true;

      frame_refresh_timer_->Tick += gcnew System::EventHandler(this, &OpenGLModelDisplayer::GLForm::FrameRefreshTimerTick);
      frame_refresh_timer_->Start();
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
    System::Windows::Forms::Panel ^gl_panel_;
    System::Windows::Forms::Label ^hint_label_;
    System::Windows::Forms::Timer ^frame_refresh_timer_;
    System::Windows::Forms::MenuStrip ^form_menu_strip_;
    System::Windows::Forms::ToolStripMenuItem ^file_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^open_obj_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^mode_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^view_basic_demo_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^view_obj_tool_strip_menu_item_;
    System::Windows::Forms::ToolStripMenuItem ^view_robot_tool_strip_menu_item_;
    System::Windows::Forms::Button ^button_switch_rotate_mode_;
    System::Windows::Forms::Button^  button_reset_eye_;
    System::ComponentModel::IContainer ^components;

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
      hwnd = (HWND)gl_panel_->Handle.ToInt32();
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

      shader_uniform_texture_flag_id = glGetUniformLocation(shader_program_id, SHADER_UNIFORM_TEXTURE_FLAG_NAME.c_str());
      if (shader_uniform_texture_flag_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_UNIFORM_TEXTURE_FLAG_NAME << ".\n";
      }
    }

    void RenderGLPanel() {
      wglMakeCurrent(hdc, hrc);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      float aspect_ratio = gl_panel_->Size.Width / (float)gl_panel_->Size.Height;

      glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.01f, 1000.0f);

      glm::mat4 view_matrix = glm::lookAt(eye_position * eye_position_scale, look_at_position, glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 modelview_matrix = glm::mat4(1.0f);

      if (rotate_view_matrix) {
        view_matrix = glm::rotate(view_matrix, glm::radians(rotated_degree.x), glm::vec3(1, 0, 0));
        view_matrix = glm::rotate(view_matrix, glm::radians(rotated_degree.y), glm::vec3(0, 1, 0));
        view_matrix = glm::rotate(view_matrix, glm::radians(rotated_degree.z), glm::vec3(0, 0, 1));
      } else {
        modelview_matrix = glm::rotate(modelview_matrix, glm::radians(rotated_degree.x), glm::vec3(1, 0, 0));
        modelview_matrix = glm::rotate(modelview_matrix, glm::radians(rotated_degree.y), glm::vec3(0, 1, 0));
        modelview_matrix = glm::rotate(modelview_matrix, glm::radians(rotated_degree.z), glm::vec3(0, 0, 1));
      }

      glUseProgram(shader_program_id);

      glUniformMatrix4fv(shader_uniform_projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
      glUniformMatrix4fv(shader_uniform_view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));

      if (program_status == VIEW_BASIC_DEMO) {
        basic_demo_model.Draw(modelview_matrix);
      } if (program_status == VIEW_OBJ) {
        obj_model.Draw(modelview_matrix);
      } else if (program_status == VIEW_ROBOT) {
        robot.Draw(modelview_matrix);
      }

      SwapBuffers(hdc);
    }

    System::Void FrameRefreshTimerTick(System::Object ^sender, System::EventArgs ^e) {
      RenderGLPanel();

      std::chrono::steady_clock::time_point current_clock = std::chrono::steady_clock::now();
      float elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_clock - last_clock).count();
      last_clock = current_clock;

      rotated_degree = rotated_degree + ROTATED_DEGREE_PER_SECOND * (elapsed_time / 1000.0f);

      if (program_status == VIEW_BASIC_DEMO) {
      } else if (program_status == VIEW_OBJ) {
      } else if (program_status == VIEW_ROBOT) {
        robot.Update(elapsed_time);
      }
    }

    System::Void GLPanelKeyDown(System::Object ^sender, System::Windows::Forms::KeyEventArgs ^e) {
      switch (e->KeyCode) {
      case Keys::F1:
        ResetEye();
        break;
      }

      if (program_status == VIEW_BASIC_DEMO) {
      } else if (program_status == VIEW_OBJ) {
      } else if (program_status == VIEW_ROBOT) {
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

    System::Void OpenObjButtonClick(System::Object ^sender, System::EventArgs ^e) {
      OpenFileDialog ^open_obj_file_dialog = gcnew OpenFileDialog();
      open_obj_file_dialog->Filter = "Obj files | *.obj";
      open_obj_file_dialog->Title = "Open a obj file.";

      if (open_obj_file_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        obj_model.BuildMeshFromObjFile(msclr::interop::marshal_as<std::string>(open_obj_file_dialog->FileName));
        ChangeProgramStatus(VIEW_OBJ);
      }
    }

    System::Void ViewBasicDemoButtonClick(System::Object ^sender, System::EventArgs ^e) {
      ChangeProgramStatus(VIEW_BASIC_DEMO);
    }

    System::Void ViewObjButtonClick(System::Object ^sender, System::EventArgs ^e) {
      ChangeProgramStatus(VIEW_OBJ);
    }

    System::Void ViewRobotButtonClick(System::Object ^sender, System::EventArgs ^e) {
      ChangeProgramStatus(VIEW_ROBOT);
    }

    System::Void SwitchRotateModeButtonClick(System::Object ^sender, System::EventArgs ^e) {
      rotate_view_matrix = !rotate_view_matrix;
      button_switch_rotate_mode_->Text = rotate_view_matrix ? "Rotate view" : "Rotate model";
    }

    System::Void ResetEyeButtonClick(System::Object ^sender, System::EventArgs ^e) {
      ResetEye();
    }

    void ResetEye() {
      eye_position = DEFAULT_EYE_POSITION;
      eye_position_scale = 1.0;
      rotated_degree = glm::vec3(0, 0, 0);
    }

    void ChangeProgramStatus(const ProgramStatus &new_program_status) {
      program_status = new_program_status;

      if (program_status == VIEW_BASIC_DEMO) {
        hint_label_->Text = " Press mouse : Adjust view angle \n Scroll mouse : Change eyes distance \n ";
      } else if (program_status == VIEW_OBJ) {
        hint_label_->Text = " Press mouse : Adjust view angle \n Scroll mouse : Change eyes distance \n ";
      } else if (program_status == VIEW_ROBOT) {
        hint_label_->Text = " Press mouse : Adjust view angle \n Scroll mouse : Change eyes distance \n F1 : Stand \n F2 : Walk \n F3 : Run \n F4 : Kungfu \n F5 : Spike";
      }

      ResetEye();
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
      this->gl_panel_ = (gcnew System::Windows::Forms::Panel());
      this->frame_refresh_timer_ = (gcnew System::Windows::Forms::Timer(this->components));
      this->hint_label_ = (gcnew System::Windows::Forms::Label());
      this->form_menu_strip_ = (gcnew System::Windows::Forms::MenuStrip());
      this->file_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->open_obj_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->mode_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->view_basic_demo_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->view_obj_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->view_robot_tool_strip_menu_item_ = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->button_switch_rotate_mode_ = (gcnew System::Windows::Forms::Button());
      this->button_reset_eye_ = (gcnew System::Windows::Forms::Button());
      this->form_menu_strip_->SuspendLayout();
      this->SuspendLayout();
      // 
      // gl_panel_
      // 
      this->gl_panel_->Location = System::Drawing::Point(50, 50);
      this->gl_panel_->Name = L"gl_panel_";
      this->gl_panel_->Size = System::Drawing::Size(800, 600);
      this->gl_panel_->TabIndex = 0;
      // 
      // hint_label_
      // 
      this->hint_label_->AutoSize = true;
      this->hint_label_->Location = System::Drawing::Point(900, 50);
      this->hint_label_->Name = L"hint_label_";
      this->hint_label_->Size = System::Drawing::Size(52, 13);
      this->hint_label_->TabIndex = 1;
      this->hint_label_->Text = L"hint_label";
      // 
      // form_menu_strip_
      // 
      this->form_menu_strip_->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->file_tool_strip_menu_item_, 
        this->mode_tool_strip_menu_item_});
      this->form_menu_strip_->Location = System::Drawing::Point(0, 0);
      this->form_menu_strip_->Name = L"form_menu_strip_";
      this->form_menu_strip_->Size = System::Drawing::Size(1184, 24);
      this->form_menu_strip_->TabIndex = 2;
      this->form_menu_strip_->Text = L"menuStrip1";
      // 
      // file_tool_strip_menu_item_
      // 
      this->file_tool_strip_menu_item_->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->open_obj_tool_strip_menu_item_});
      this->file_tool_strip_menu_item_->Name = L"file_tool_strip_menu_item_";
      this->file_tool_strip_menu_item_->Size = System::Drawing::Size(37, 20);
      this->file_tool_strip_menu_item_->Text = L"File";
      // 
      // open_obj_tool_strip_menu_item_
      // 
      this->open_obj_tool_strip_menu_item_->Name = L"open_obj_tool_strip_menu_item_";
      this->open_obj_tool_strip_menu_item_->Size = System::Drawing::Size(123, 22);
      this->open_obj_tool_strip_menu_item_->Text = L"Open obj";
      // 
      // mode_tool_strip_menu_item_
      // 
      this->mode_tool_strip_menu_item_->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->view_basic_demo_tool_strip_menu_item_, 
        this->view_obj_tool_strip_menu_item_, this->view_robot_tool_strip_menu_item_});
      this->mode_tool_strip_menu_item_->Name = L"mode_tool_strip_menu_item_";
      this->mode_tool_strip_menu_item_->Size = System::Drawing::Size(50, 20);
      this->mode_tool_strip_menu_item_->Text = L"Mode";
      // 
      // view_basic_demo_tool_strip_menu_item_
      // 
      this->view_basic_demo_tool_strip_menu_item_->Name = L"view_basic_demo_tool_strip_menu_item_";
      this->view_basic_demo_tool_strip_menu_item_->Size = System::Drawing::Size(163, 22);
      this->view_basic_demo_tool_strip_menu_item_->Text = L"View basic demo";
      // 
      // view_obj_tool_strip_menu_item_
      // 
      this->view_obj_tool_strip_menu_item_->Name = L"view_obj_tool_strip_menu_item_";
      this->view_obj_tool_strip_menu_item_->Size = System::Drawing::Size(163, 22);
      this->view_obj_tool_strip_menu_item_->Text = L"View obj";
      // 
      // view_robot_tool_strip_menu_item_
      // 
      this->view_robot_tool_strip_menu_item_->Name = L"view_robot_tool_strip_menu_item_";
      this->view_robot_tool_strip_menu_item_->Size = System::Drawing::Size(163, 22);
      this->view_robot_tool_strip_menu_item_->Text = L"View robot";
      // 
      // button_switch_rotate_mode_
      // 
      this->button_switch_rotate_mode_->Location = System::Drawing::Point(900, 250);
      this->button_switch_rotate_mode_->Name = L"button_switch_rotate_mode_";
      this->button_switch_rotate_mode_->Size = System::Drawing::Size(125, 25);
      this->button_switch_rotate_mode_->TabIndex = 3;
      this->button_switch_rotate_mode_->Text = L"Rotate view";
      this->button_switch_rotate_mode_->UseVisualStyleBackColor = true;
      // 
      // button_reset_eye_
      // 
      this->button_reset_eye_->Location = System::Drawing::Point(900, 285);
      this->button_reset_eye_->Name = L"button_reset_eye_";
      this->button_reset_eye_->Size = System::Drawing::Size(125, 25);
      this->button_reset_eye_->TabIndex = 4;
      this->button_reset_eye_->Text = L"Reset eye (F1)";
      this->button_reset_eye_->UseVisualStyleBackColor = true;
      // 
      // GLForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(1184, 712);
      this->Controls->Add(this->button_reset_eye_);
      this->Controls->Add(this->button_switch_rotate_mode_);
      this->Controls->Add(this->hint_label_);
      this->Controls->Add(this->gl_panel_);
      this->Controls->Add(this->form_menu_strip_);
      this->KeyPreview = true;
      this->MainMenuStrip = this->form_menu_strip_;
      this->Name = L"GLForm";
      this->Text = L"GLForm";
      this->Load += gcnew System::EventHandler(this, &GLForm::GLForm_Load);
      this->form_menu_strip_->ResumeLayout(false);
      this->form_menu_strip_->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  private:

    System::Void GLForm_Load(System::Object ^sender, System::EventArgs ^e) {
    }
  };
}
