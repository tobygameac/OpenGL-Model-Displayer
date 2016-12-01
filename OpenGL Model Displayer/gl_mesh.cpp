#pragma once

#include "gl_mesh.h"

GLMesh::GLMesh() : vbo_vertices_(0), vbo_colors_(0), vbo_normals_(0), vbo_uvs_(0), local_modelview_matrix_(glm::mat4(1.0)), texture_id_(0), texture_flag_(false) {
}

GLMesh::GLMesh(const GLMesh &other) {
  DeepCopy(other);
}

GLMesh::~GLMesh() {
  if (vbo_vertices_) {
    glDeleteBuffers(1, &vbo_vertices_);
  }
  if (vbo_colors_) {
    glDeleteBuffers(1, &vbo_colors_);
  }
  if (vbo_normals_) {
    glDeleteBuffers(1, &vbo_normals_);
  }
  if (vbo_uvs_) {
    glDeleteBuffers(1, &vbo_uvs_);
  }
}

GLMesh &GLMesh::operator=(const GLMesh &other) {
  DeepCopy(other);
  return *this;
}

void GLMesh::DeepCopy(const GLMesh &other) {
  vertices_ = other.vertices_;
  colors_ = other.colors_;
  normals_ = other.normals_;
  uvs_ = other.uvs_;

  texture_flag_ = other.texture_flag_;

  texture_id_ = other.texture_id_;

  local_modelview_matrix_ = other.local_modelview_matrix_;

  vbo_vertices_ = vbo_colors_ = vbo_normals_ = vbo_uvs_ = 0;

  this->Upload();
}

void GLMesh::AddCube(std::shared_ptr<GLMesh> mesh, const float size) {
  AddCube(mesh, glm::vec3(0, 0, 0), size, size, size);
}

void GLMesh::AddCube(std::shared_ptr<GLMesh> mesh, const float width, const float length, const float height) {
  AddCube(mesh, glm::vec3(0, 0, 0), width, length, height);
}

void GLMesh::AddCube(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float size) {
  AddCube(mesh, center, size, size, size);
}

void GLMesh::AddCube(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float width, const float length, const float height) {
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, 1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, -1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, -1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, -1 * length));

  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, -1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, -1 * length));

  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, 1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, -1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, 1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, -1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, 1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * height, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, -1 * height, 1 * length));


  for (size_t i = 0; i < mesh->vertices_.size(); ++i) {
    mesh->vertices_[i] *= 0.5;
    mesh->vertices_[i] += center;
  }
}

void GLMesh::AddCone(std::shared_ptr<GLMesh> mesh, const float size) {
  AddCone(mesh, glm::vec3(0, 0, 0), size, size, size);
}

void GLMesh::AddCone(std::shared_ptr<GLMesh> mesh, const float width, const float length, const float height) {
  AddCone(mesh, glm::vec3(0, 0, 0), width, length, height);
}

void GLMesh::AddCone(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float size) {
  AddCone(mesh, center, size, size, size);
}

void GLMesh::AddCone(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float width, const float length, const float height) {
  mesh->vertices_.push_back(glm::vec3(-1 * width, 0, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 0, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, 0, 1 * length));

  mesh->vertices_.push_back(glm::vec3(1 * width, 0, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, 0, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 0, -1 * length));

  mesh->vertices_.push_back(glm::vec3(0, height, 0));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 0, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 0, 1 * length));

  mesh->vertices_.push_back(glm::vec3(0, height, 0));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 0, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, 0, 1 * length));

  mesh->vertices_.push_back(glm::vec3(0, height, 0));
  mesh->vertices_.push_back(glm::vec3(1 * width, 0, 1 * length));
  mesh->vertices_.push_back(glm::vec3(1 * width, 0, -1 * length));

  mesh->vertices_.push_back(glm::vec3(0, height, 0));
  mesh->vertices_.push_back(glm::vec3(1 * width, 0, -1 * length));
  mesh->vertices_.push_back(glm::vec3(-1 * width, 0, -1 * length));

  for (size_t i = 0; i < mesh->vertices_.size(); ++i) {
    mesh->vertices_[i].x *= 0.5;
    mesh->vertices_[i].y -= height * 0.5;
    mesh->vertices_[i].z *= 0.5;
    mesh->vertices_[i] += center;
  }
}

glm::mat4 GLMesh::GetModelviewMatrixWithAnimation() {
  glm::mat4 modelview_matrix_with_animation = local_modelview_matrix_;

  glm::vec3 rotation_animation_status = rotation_animation_.AnimationStatus();
  glm::vec3 translation_animation_status = translation_animation_.AnimationStatus();

  modelview_matrix_with_animation = glm::rotate(modelview_matrix_with_animation, glm::radians(rotation_animation_status.x), glm::vec3(1, 0, 0));
  modelview_matrix_with_animation = glm::rotate(modelview_matrix_with_animation, glm::radians(rotation_animation_status.y), glm::vec3(0, 1, 0));
  modelview_matrix_with_animation = glm::rotate(modelview_matrix_with_animation, glm::radians(rotation_animation_status.z), glm::vec3(0, 0, 1));

  modelview_matrix_with_animation = glm::translate(modelview_matrix_with_animation, translation_animation_status);

  return modelview_matrix_with_animation;
}

void GLMesh::UpdateAnimationStatus(const float delta_time) {
  rotation_animation_.Update(delta_time);
  translation_animation_.Update(delta_time);
}

glm::vec3 GLMesh::AlignPositionToOrigin(const glm::vec3 parent_translation_vector) {

  if (!vertices_.size()) {
    return glm::vec3(0, 0, 0);
  }

  glm::vec3 center_position(0, 0, 0);
  for (const auto &vertex : vertices_) {
    center_position += vertex;
  }
  center_position /= vertices_.size();

  for (auto &vertex : vertices_) {
    vertex -= center_position;
  }

  glm::vec3 adjust_amount = center_position - parent_translation_vector;
  Translate(adjust_amount);
  return adjust_amount;
}

void GLMesh::Translate(const glm::vec3 &translation_vector) {
  local_modelview_matrix_ = glm::translate(local_modelview_matrix_, translation_vector);
}

void GLMesh::Rotate(const float rotation_angle, const glm::vec3 &rotation_axis) {
  local_modelview_matrix_ = glm::rotate(local_modelview_matrix_, rotation_angle, rotation_axis);
}

void GLMesh::SetColor(const glm::vec3 &color) {
  colors_ = std::vector<glm::vec3>(vertices_.size(), color);
}

void GLMesh::SetRandomColors() {
  colors_ = std::vector<glm::vec3>(vertices_.size());
  for (auto &color : colors_) {
    color = glm::vec3(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
  }
}

void GLMesh::SetNormal() {
  normals_.resize(vertices_.size());
  for (size_t i = 0; i < vertices_.size(); i += 3) {
    normals_[i] = normals_[i + 1] = normals_[i + 2] = glm::normalize(glm::cross(vertices_[i + 1] - vertices_[i], vertices_[i + 2] - vertices_[i]));
  }
}

void GLMesh::Upload() {
  if (vertices_.size()) {
    if (!vbo_vertices_) {
      glGenBuffers(1, &vbo_vertices_);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertices_[0]), vertices_.data(), GL_STATIC_DRAW);
  }

  if (colors_.size()) {
    if (!vbo_colors_) {
      glGenBuffers(1, &vbo_colors_);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors_);
    glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(colors_[0]), colors_.data(), GL_STATIC_DRAW);
  }

  if (normals_.size()) {
    if (!vbo_normals_) {
      glGenBuffers(1, &vbo_normals_);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_);
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(normals_[0]), normals_.data(), GL_STATIC_DRAW);
  }

  if (uvs_.size()) {
    if (!vbo_uvs_) {
      glGenBuffers(1, &vbo_uvs_);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs_);
    glBufferData(GL_ARRAY_BUFFER, uvs_.size() * sizeof(uvs_[0]), uvs_.data(), GL_STATIC_DRAW);
  }

  texture_flag_ = uvs_.size();
}

void GLMesh::Draw(const GLShader &gl_shader) {
  Draw(gl_shader, glm::mat4(1.0f));
}

void GLMesh::Draw(const GLShader &gl_shader, const glm::mat4 &parent_modelview_matrix) {

  if (vbo_vertices_) {
    glEnableVertexAttribArray(gl_shader.shader_attribute_vertex_position_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
    glVertexAttribPointer(gl_shader.shader_attribute_vertex_position_id_, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  if (vbo_colors_) {
    glEnableVertexAttribArray(gl_shader.shader_attribute_vertex_color_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors_);
    glVertexAttribPointer(gl_shader.shader_attribute_vertex_color_id_, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  if (vbo_normals_) {
    glEnableVertexAttribArray(gl_shader.shader_attribute_vertex_normal_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_);
    glVertexAttribPointer(gl_shader.shader_attribute_vertex_normal_id_, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }

  if (vbo_uvs_) {
    glEnableVertexAttribArray(gl_shader.shader_attribute_vertex_uv_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs_);
    glVertexAttribPointer(gl_shader.shader_attribute_vertex_uv_id_, 2, GL_FLOAT, GL_FALSE, 0, 0);
  }

  if (texture_flag_) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glUniform1i(gl_shader.shader_uniform_texture_id_, 0);
  }

  glUniform1f(gl_shader.shader_uniform_texture_flag_id_, texture_flag_ ? 1.0 : 0.0);

  glm::mat4 modelview_matrix = parent_modelview_matrix * GetModelviewMatrixWithAnimation();

  glUniformMatrix4fv(gl_shader.shader_uniform_modelview_matrix_id_, 1, GL_FALSE, glm::value_ptr(modelview_matrix));

  glm::mat4 inverse_modelview_matrix = glm::inverse(modelview_matrix);
  glUniformMatrix3fv(gl_shader.shader_uniform_inverse_modelview_matrix_id_, 1, GL_FALSE, glm::value_ptr(inverse_modelview_matrix));

  glm::mat3 transpose_inverse_modelview_matrix = glm::transpose(glm::inverse(glm::mat3(modelview_matrix)));
  glUniformMatrix3fv(gl_shader.shader_uniform_transpose_inverse_modelview_matrix_id_, 1, GL_FALSE, glm::value_ptr(transpose_inverse_modelview_matrix));

  glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

  if (vbo_vertices_) {
    glDisableVertexAttribArray(gl_shader.shader_attribute_vertex_position_id_);
  }

  if (vbo_colors_) {
    glDisableVertexAttribArray(gl_shader.shader_attribute_vertex_color_id_);
  }

  if (vbo_normals_) {
    glDisableVertexAttribArray(gl_shader.shader_attribute_vertex_normal_id_);
  }

  if (vbo_uvs_) {
    glDisableVertexAttribArray(gl_shader.shader_attribute_vertex_uv_id_);
  }

}