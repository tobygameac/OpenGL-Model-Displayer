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

  const std::string SHADER_VERTEX_POSITION_ATTRIBUTE_NAME = "vertex_position";
  const std::string SHADER_VERTEX_COLOR_ATTRIBUTE_NAME = "vertex_color";
  const std::string SHADER_VERTEX_NORMAL_ATTRIBUTE_NAME = "vertex_normal";

  const std::string SHADER_MODEL_MATRIX_UNIFORM_NAME = "model_matrix";
  const std::string SHADER_VIEW_MATRIX_UNIFORM_NAME = "view_matrix";
  const std::string SHADER_PROJECTION_MATRIX_UNIFORM_NAME = "projection_matrix";

  const std::string DEFAULT_MODEL_OBJ_FILE_PATH = "..\\data\\Robo8.obj";

  GLint shader_program_id;
  GLint shader_attribute_vertex_position_id;
  GLint shader_attribute_vertex_color_id;
  GLint shader_attribute_vertex_normal_id;
  GLint shader_uniform_model_matrix_id;
  GLint shader_uniform_view_matrix_id;
  GLint shader_uniform_projection_matrix_id;

  const int FPS = 120;
  const int FRAME_REFRESH_TIME = (int)(1000.0 / FPS);

  glm::vec3 eye_position(10.0, 10.0, 10.0);

  const float EYE_POSITION_SCALE_PER_SCROLLING = 0.9;
  float eye_position_scale = 1.0;
  float rotated_angle;

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

      timer1->Interval = FRAME_REFRESH_TIME;
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

    static bool ParseObjFileIntoMesh(const std::string &file_path, std::shared_ptr<GLMesh> mesh) {

      if (mesh == nullptr) {
        return false;
      }

      std::ifstream file_stream(file_path);
      if (!file_stream.is_open()) {
        return false;
      }


      std::string line_buffer;

      std::vector<glm::vec3> temp_vertices;

      glm::vec3 color(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);

      while (std::getline(file_stream, line_buffer)) {
        std::istringstream input_string_stream(line_buffer);
        std::string line_header;
        input_string_stream >> line_header;

        if (line_header == "#") {
          continue;
        } else if (line_header == "mtllib") {
          continue;
        } else if (line_header == "usemtl") {
          color.x = rand() / (float)RAND_MAX;
          color.y = rand() / (float)RAND_MAX;
          color.z = rand() / (float)RAND_MAX;
          continue;
        } else if (line_header == "o") {
          //temp_vertices.clear();
          continue;
        } else if (line_header == "s") {
          continue;
        } else if (line_header == "v") {
          glm::vec3 vertex;
          input_string_stream >> vertex.x >> vertex.y >> vertex.z;
          //std::cout << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
          temp_vertices.push_back(vertex);
        } else if (line_header == "f") {
          std::string vertex_detail;
          while (input_string_stream >> vertex_detail) {
            std::replace(vertex_detail.begin(), vertex_detail.end(), '/', ' ');
            std::istringstream vertex_detail_string_stream(vertex_detail);
            size_t vertex_index, vt_index, vn_index;
            vertex_detail_string_stream >> vertex_index;
            mesh->vertices_.push_back(temp_vertices[vertex_index - 1]);
            mesh->colors_.push_back(color);
            if (vertex_detail_string_stream) {
              vertex_detail_string_stream >> vt_index;
            }
            if (vertex_detail_string_stream) {
              vertex_detail_string_stream >> vn_index;
            }
          }
          continue;
        } else if (line_header == "vn") {
          continue;
        } else if (line_header == "vt") {
          continue;
        } else if (line_header == "vp") {
          continue;
        } else {
          continue;
        }
      }

      mesh->Upload();

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

      shader_attribute_vertex_position_id = glGetAttribLocation(shader_program_id, SHADER_VERTEX_POSITION_ATTRIBUTE_NAME.c_str());
      if (shader_attribute_vertex_position_id == -1) {
        std::cerr << "Could not bind attribute " << SHADER_VERTEX_POSITION_ATTRIBUTE_NAME << ".\n";
      }

      shader_attribute_vertex_color_id = glGetAttribLocation(shader_program_id, SHADER_VERTEX_COLOR_ATTRIBUTE_NAME.c_str());
      if (shader_attribute_vertex_color_id == -1) {
        std::cerr << "Could not bind attribute " << SHADER_VERTEX_COLOR_ATTRIBUTE_NAME << ".\n";
      }

      shader_attribute_vertex_normal_id = glGetAttribLocation(shader_program_id, SHADER_VERTEX_NORMAL_ATTRIBUTE_NAME.c_str());
      if (shader_attribute_vertex_normal_id == -1) {
        std::cerr << "Could not bind attribute " << SHADER_VERTEX_NORMAL_ATTRIBUTE_NAME << ".\n";
      }

      shader_uniform_model_matrix_id = glGetUniformLocation(shader_program_id, SHADER_MODEL_MATRIX_UNIFORM_NAME.c_str());

      if (shader_uniform_model_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_MODEL_MATRIX_UNIFORM_NAME << ".\n.";
      }

      shader_uniform_view_matrix_id = glGetUniformLocation(shader_program_id, SHADER_VIEW_MATRIX_UNIFORM_NAME.c_str());

      if (shader_uniform_view_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_VIEW_MATRIX_UNIFORM_NAME << ".\n.";
      }

      shader_uniform_projection_matrix_id = glGetUniformLocation(shader_program_id, SHADER_PROJECTION_MATRIX_UNIFORM_NAME.c_str());

      if (shader_uniform_projection_matrix_id == -1) {
        std::cerr << "Could not bind uniform " << SHADER_PROJECTION_MATRIX_UNIFORM_NAME << ".\n.";
      }

      //ParseObjFileIntoMesh(DEFAULT_MODEL_OBJ_FILE_PATH, robot.root_mesh_node_->mesh_);

      robot.BuildSimpleMesh();
    }

    void RenderGLPanel() {
      wglMakeCurrent(hdc, hrc);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glm::mat4 projection_matrix = glm::perspective(45.0f, 1.0f / 1.0f, 0.1f, 100.0f);

      glm::mat4 view_matrix = glm::lookAt(eye_position * eye_position_scale, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

      glm::mat4 model_matrix = glm::mat4(1.0f);
      model_matrix = glm::rotate(model_matrix, (float)(rotated_angle * acos(-1) / 180.0), glm::vec3(0, 1, 0));
      //model_matrix = glm::translate(model_matrix, glm::vec3(0, (int)rotated_angle / 360.0f, 0));

      glUseProgram(shader_program_id);

      glUniformMatrix4fv(shader_uniform_projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
      glUniformMatrix4fv(shader_uniform_view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));

      robot.Draw(model_matrix);

      SwapBuffers(hdc);
    }

    System::Void Timer1Tick(System::Object^ sender, System::EventArgs^ e) {
      rotated_angle = rotated_angle + 1;
      rotated_angle = rotated_angle - ((rotated_angle > 360) ? 360 : 0);

      RenderGLPanel();

      robot.head_mesh_node_->mesh_->Rotate(0.05, glm::vec3(0, 1, 0));
      robot.left_upper_arm_mesh_node_->mesh_->Rotate(0.05, glm::vec3(1, 0, 0));
      robot.right_lower_arm_mesh_node_->mesh_->Rotate(0.05, glm::vec3(0, 1, 0));
      robot.right_upper_leg_mesh_node_->mesh_->Rotate(0.05, glm::vec3(1, 0, 0));
    }

    System::Void GLPanelMouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
      eye_position_scale *= (e->Delta < 0) ? (1.0 / EYE_POSITION_SCALE_PER_SCROLLING) : EYE_POSITION_SCALE_PER_SCROLLING;
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

    System::Void GLForm_Load(System::Object^  sender, System::EventArgs^  e) {}
  };
}
