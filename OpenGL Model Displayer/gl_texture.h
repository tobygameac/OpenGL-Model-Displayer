#pragma once

#include <GL\glew.h>
#include <opencv\cv.hpp>

namespace GLTexture {

  void SetGLTexture(const cv::Mat &cv_image, GLuint *texture_id);

  void SetGLTexture(void *image_data_pointer, int width, int height, GLuint *texture_id);

};