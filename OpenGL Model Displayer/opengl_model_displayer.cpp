#include "opengl_model_displayer.h"

OpenGLModelDisplayer::OpenGLModelDisplayer(QWidget *parent)
  : QMainWindow(parent) {
  ui.setupUi(this);

  ui.frame_gl_canvas_->installEventFilter(this);

  srand((unsigned)time(0));

  InitializeOpenGL();

  basic_demo_model.BuildBasicDemoShape();

  if (!robot.BuildMeshFromOptimusPrimeObjFile(ROBOT_MODEL_OBJ_FILE_PATH)) {
    robot.BuildSimpleMesh();
  }

  ChangeProgramStatus(VIEW_ROBOT);

  connect(ui.action_open_obj_, SIGNAL(triggered()),
    this, SLOT(OnActionTriggeredOpenObj()));

  connect(ui.action_load_texture, SIGNAL(triggered()),
    this, SLOT(OnActionTriggeredLoadTexture()));

  connect(ui.action_view_basic_demo_, SIGNAL(triggered()),
    this, SLOT(OnActionTriggeredViewBasicDemo()));

  connect(ui.action_view_obj_, SIGNAL(triggered()),
    this, SLOT(OnActionTriggeredViewObj()));

  connect(ui.action_view_robot_, SIGNAL(triggered()),
    this, SLOT(OnActionTriggeredViewRobot()));

  connect(ui.button_switch_rotate_mode_, SIGNAL(clicked()),
    this, SLOT(OnButtonClickedSwitchRotateMode()));

  connect(ui.button_reset_eye_, SIGNAL(clicked()),
    this, SLOT(OnButtonClickedResetEye()));

  connect(&q_timer, SIGNAL(timeout()),
    this, SLOT(RefreshFrame()));

  q_timer.start(FRAME_REFRESH_TIME);
}

void OpenGLModelDisplayer::InitializeOpenGL() {

  // Get Handle
  hwnd = (HWND)ui.frame_gl_canvas_->winId();
  hdc = GetDC(hwnd);

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

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  gl_shader.CreateDefaultShader();
}

void OpenGLModelDisplayer::RenderingOnGLCanvas() {
  //wglMakeCurrent(hdc, hrc);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glClearColor(0.5, 0.5, 0.5, 1);

  float aspect_ratio = ui.frame_gl_canvas_->width() / (float)ui.frame_gl_canvas_->height();

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

  glUseProgram(gl_shader.shader_program_id_);

  glUniformMatrix4fv(gl_shader.shader_uniform_projection_matrix_id_, 1, GL_FALSE, glm::value_ptr(projection_matrix));
  glUniformMatrix4fv(gl_shader.shader_uniform_view_matrix_id_, 1, GL_FALSE, glm::value_ptr(view_matrix));

  if (program_status == VIEW_BASIC_DEMO) {
    basic_demo_model.Draw(gl_shader, modelview_matrix);
  } if (program_status == VIEW_OBJ) {
    for (GLSimpleModel &obj_model : obj_models) {
      obj_model.Draw(gl_shader, modelview_matrix);
    }
  } else if (program_status == VIEW_ROBOT) {
    robot.Draw(gl_shader, modelview_matrix);
  }

  SwapBuffers(hdc);
}

void OpenGLModelDisplayer::OnActionTriggeredOpenObj() {
  QStringList q_file_paths = QFileDialog::getOpenFileNames(this,
    tr("Open obj files."), "..//data//", tr("Obj Files (*.obj)"));

  if (q_file_paths.isEmpty()) {
    return;
  }

  obj_models.clear();

  std::vector<std::string> obj_paths;
  for (const QString &q_file_path : q_file_paths) {
    std::string obj_path = q_file_path.toUtf8().constData();
    GLSimpleModel obj_model;
    obj_model.BuildMeshFromObjFile(obj_path);
    obj_models.push_back(obj_model);
    ChangeProgramStatus(VIEW_OBJ);
  }
}

void OpenGLModelDisplayer::OnActionTriggeredLoadTexture() {
  QString q_file_path = QFileDialog::getOpenFileName(this,
    tr("Open an image file."), "..//data//", tr("Image Files (*.*)"));

  if (q_file_path.isEmpty()) {
    return;
  }

  std::string file_path = q_file_path.toUtf8().constData();

  cv::Mat texture = cv::imread(file_path);
  GLuint texture_id;
  GLTexture::SetGLTexture(texture, &texture_id);
  for (GLSimpleModel &obj_model : obj_models) {
    obj_model.SetTextureID(texture_id);
  }

}

void OpenGLModelDisplayer::OnActionTriggeredViewBasicDemo() {
  ChangeProgramStatus(VIEW_BASIC_DEMO);

}

void OpenGLModelDisplayer::OnActionTriggeredViewObj() {
  ChangeProgramStatus(VIEW_OBJ);
}

void OpenGLModelDisplayer::OnActionTriggeredViewRobot() {
  ChangeProgramStatus(VIEW_ROBOT);
}

void OpenGLModelDisplayer::OnButtonClickedSwitchRotateMode() {
  rotate_view_matrix = !rotate_view_matrix;
  //button_switch_rotate_mode_->Text = rotate_view_matrix ? "Rotate view" : "Rotate model";
}

void OpenGLModelDisplayer::OnButtonClickedResetEye() {
  ResetEye();
}

void OpenGLModelDisplayer::RefreshFrame() {
  RenderingOnGLCanvas();

  std::chrono::high_resolution_clock::time_point current_clock = std::chrono::steady_clock::now();
  float elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_clock - last_clock).count();
  last_clock = current_clock;

  rotated_degree = rotated_degree + ROTATED_DEGREE_PER_SECOND * (elapsed_time / 1000.0f);

  if (program_status == VIEW_BASIC_DEMO) {
  } else if (program_status == VIEW_OBJ) {
  } else if (program_status == VIEW_ROBOT) {
    robot.Update(elapsed_time);
  }
}

void OpenGLModelDisplayer::ResetEye() {
  eye_position = DEFAULT_EYE_POSITION;
  eye_position_scale = 1.0;
  rotated_degree = glm::vec3(0, 0, 0);
}

void OpenGLModelDisplayer::ChangeProgramStatus(const ProgramStatus &new_program_status) {
  program_status = new_program_status;

  if (program_status == VIEW_BASIC_DEMO) {
    ui.label_hint_->setText(" Press mouse : Adjust view angle \n Scroll mouse : Change eyes distance \n ");
  } else if (program_status == VIEW_OBJ) {
    ui.label_hint_->setText(" Press mouse : Adjust view angle \n Scroll mouse : Change eyes distance \n ");
  } else if (program_status == VIEW_ROBOT) {
    ui.label_hint_->setText(" Press mouse : Adjust view angle \n Scroll mouse : Change eyes distance \n F1 : Stand \n F2 : Walk \n F3 : Run \n F4 : Kungfu \n F5 : Spike");
  }

  ResetEye();
}

bool OpenGLModelDisplayer::eventFilter(QObject *object, QEvent *q_event) {
  if (object == this) {

  } else if (object == ui.frame_gl_canvas_) {
    if (q_event->type() == QEvent::MouseMove) {
      QMouseEvent *mouse_event = static_cast<QMouseEvent *>(q_event);

      if (is_dragging_panel) {
        rotated_degree.y += (mouse_event->x() - mouse_last_x);
        rotated_degree.x += (mouse_event->y() - mouse_last_y);
      }

      mouse_last_x = mouse_event->x();
      mouse_last_y = mouse_event->y();

      RefreshFrame();
    } else if (q_event->type() == QEvent::MouseButtonPress) {
      is_dragging_panel = true;
    } else if (q_event->type() == QEvent::MouseButtonRelease) {
      is_dragging_panel = false;
    } else if (q_event->type() == QEvent::Wheel) {
      QWheelEvent *wheel_event = static_cast<QWheelEvent *>(q_event);
      double wheel_amount = wheel_event->angleDelta().y();
      eye_position_scale *= (wheel_amount < 0) ? (1.0f / EYE_POSITION_SCALE_PER_SCROLLING) : EYE_POSITION_SCALE_PER_SCROLLING;

      RefreshFrame();
    } else if (q_event->type() == QEvent::KeyPress) {
      QKeyEvent *key_event = static_cast<QKeyEvent *>(q_event);

      if (program_status == VIEW_BASIC_DEMO) {
      } else if (program_status == VIEW_OBJ) {
      } else if (program_status == VIEW_ROBOT) {
        switch (key_event->key()) {
        case Qt::Key_F1:
          robot.ClearAnimation();
          break;
        case Qt::Key_F2:
        case Qt::Key_F8:
          robot.WalkingMode();
          break;
        case Qt::Key::Key_F3:
        case Qt::Key::Key_F9:
          robot.RunningMode();
          break;
        case Qt::Key::Key_F4:
          robot.KongfuMode();
          break;
        case Qt::Key::Key_F5:
          robot.SpikeMode();
          break;
        case Qt::Key::Key_W:
        case Qt::Key::Key_Up:
          eye_position.y += 1;
          look_at_position.y += 1;
          break;
        case Qt::Key::Key_A:
        case Qt::Key::Key_Left:
          eye_position.x -= 1;
          look_at_position.x -= 1;
          break;
        case Qt::Key::Key_S:
        case Qt::Key::Key_Down:
          eye_position.y -= 1;
          look_at_position.y -= 1;
          break;
        case Qt::Key::Key_D:
        case Qt::Key::Key_Right:
          eye_position.x += 1;
          look_at_position.x += 1;
          break;
        }
      }
    }
  }

  return true;
}