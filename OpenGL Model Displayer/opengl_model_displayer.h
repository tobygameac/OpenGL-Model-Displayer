#pragma once

#include <QtWidgets/QMainWindow>
#include <Qtimer>
#include <QFileDialog>
#include <QMouseEvent>

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
#include "gl_shader.h"

#include "ui_opengl_model_displayer.h"

class OpenGLModelDisplayer : public QMainWindow {
  Q_OBJECT

public:

  enum ProgramStatus {
    VIEW_BASIC_DEMO,
    VIEW_OBJ,
    VIEW_ROBOT
  };

  OpenGLModelDisplayer(QWidget *parent = Q_NULLPTR);

  private slots:

  void OnActionTriggered();

  void OnButtonClicked();

  void RefreshFrame();

private:

  void InitializeOpenGL();

  void RenderingOnGLCanvas();

  void ResetEye();

  void ChangeProgramStatus(const ProgramStatus &new_program_status);

  bool eventFilter(QObject *object, QEvent *q_event);

  HWND hwnd;
  HDC hdc;
  HGLRC hrc;

  ProgramStatus program_status;

  const std::string ROBOT_MODEL_OBJ_FILE_PATH = "..\\data\\Optimus prime\\optimus.obj";

  const int FPS = 60;
  const float FRAME_REFRESH_TIME = (1000.0 / FPS);

  const glm::vec3 DEFAULT_EYE_POSITION = glm::vec3(0, 0, 1);
  glm::vec3 eye_position = glm::vec3(DEFAULT_EYE_POSITION);
  glm::vec3 look_at_position = glm::vec3(0.0, 0.0, 0.0);

  const float EYE_POSITION_SCALE_PER_SCROLLING = 0.9f;
  float eye_position_scale = 1.0;

  const glm::vec3 ROTATED_DEGREE_PER_SECOND;// = glm::vec3(0.0, 360.0 / 8.0, 0.0);
  glm::vec3 rotated_degree;

  bool is_viewing_mesh_line = false;
  bool is_dragging_panel = false;
  bool rotate_view_matrix = false;

  GLSimpleModel basic_demo_model;
  std::vector<GLSimpleModel> obj_models;
  GLRobot robot;

  int mouse_last_x;
  int mouse_last_y;

  std::chrono::high_resolution_clock::time_point last_clock = std::chrono::high_resolution_clock::now();

  GLShader gl_shader;

  QTimer q_timer;

  Ui::OpenGLModelDisplayerClass ui;
};
