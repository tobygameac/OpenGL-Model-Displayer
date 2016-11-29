#pragma once

#include <cstdlib>

#include <memory>
#include <string>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <opencv\cv.hpp>

#include "gl_animation.h"
#include "gl_shader.h"

class GLMesh {

public:

  GLMesh();

  ~GLMesh();

  static void AddCube(std::shared_ptr<GLMesh> mesh, const float size);

  static void AddCube(std::shared_ptr<GLMesh> mesh, const float width, const float length, const float height);

  static void AddCube(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float size);

  static void AddCube(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float width, const float length, const float height);

  static void AddCone(std::shared_ptr<GLMesh> mesh, const float size);

  static void AddCone(std::shared_ptr<GLMesh> mesh, const float width, const float length, const float height);

  static void AddCone(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float size);

  static void AddCone(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float width, const float length, const float height);

  glm::mat4 GetModelviewMatrixWithAnimation();

  void UpdateAnimationStatus(const float delta_time);

  glm::vec3 AlignPositionToOrigin(const glm::vec3 parent_translation_vector);

  void Translate(const glm::vec3 &translation_vector);

  void Rotate(const float rotation_angle, const glm::vec3 &rotation_axis);

  void SetColor(const glm::vec3 &color);

  void SetRandomColors();

  void SetNormal();

  void Upload();

  void Draw(const GLShader &gl_shader);

  void Draw(const GLShader &gl_shader, const glm::mat4 &parent_modelview_matrix);

  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec3> colors_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec2> uvs_;

  GLuint texture_id_;

  Animation<glm::vec3> rotation_animation_;
  Animation<glm::vec3> translation_animation_;

private:

  GLuint vbo_vertices_;
  GLuint vbo_normals_;
  GLuint vbo_colors_;
  GLuint vbo_uvs_;

  bool texture_flag_;

  glm::mat4 local_modelview_matrix_;
};