#include "opengl_model_displayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  OpenGLModelDisplayer w;
  w.show();
  return a.exec();
}
