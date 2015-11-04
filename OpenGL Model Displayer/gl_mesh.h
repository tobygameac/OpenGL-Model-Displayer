#pragma once

#include <memory>
#include <string>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <opencv\cv.hpp>

namespace OpenGLModelDisplayer {

  extern GLint shader_program_id;
  extern GLint shader_attribute_vertex_position_id;
  extern GLint shader_attribute_vertex_color_id;
  extern GLint shader_attribute_vertex_normal_id;
  extern GLint shader_attribute_vertex_uv_id;
  extern GLint shader_uniform_modelview_matrix_id;
  extern GLint shader_uniform_view_matrix_id;
  extern GLint shader_uniform_projection_matrix_id;
  extern GLint shader_uniform_inverse_modelview_matrix_id;
  extern GLint shader_uniform_transpose_inverse_modelview_matrix_id;
  extern GLint shader_uniform_texture_id;

  class GLMesh {

  public:

    GLMesh() : vbo_vertices_(0), vbo_colors_(0), vbo_normals_(0), vbo_uvs_(0), local_modelview_matrix_(glm::mat4(1.0)), texture_id_(0) {
    }

    static void AddCube(std::shared_ptr<GLMesh> mesh, const float size) {
      AddCube(mesh, glm::vec3(0, 0, 0), size, size, size);
    }

    static void AddCube(std::shared_ptr<GLMesh> mesh, const float width, const float length, const float height) {
      AddCube(mesh, glm::vec3(0, 0, 0), width, length, height);
    }

    static void AddCube(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float size) {
      AddCube(mesh, center, size, size, size);
    }

    static void AddCube(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float width, const float length, const float height) {
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, 1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, -1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, -1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, -1 * height));

      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, -1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, -1 * height));

      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, -1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, 1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, -1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, 1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, -1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, -1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, 1 * height));

      mesh->vertices_.push_back(glm::vec3(1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(-1 * width, 1 * length, 1 * height));
      mesh->vertices_.push_back(glm::vec3(1 * width, -1 * length, 1 * height));

      for (size_t i = 0; i < mesh->vertices_.size(); ++i) {
        mesh->vertices_[i] *= 0.5;
        mesh->vertices_[i] += center;
      }
    }

    glm::mat4 GetModelviewMatrixWithAnimation() {
      glm::mat4 modelview_matrix_with_animation = local_modelview_matrix_;

      modelview_matrix_with_animation = glm::rotate(modelview_matrix_with_animation, glm::radians(rotation_animation_status_.GetCurrentStatus().x), glm::vec3(1, 0, 0));
      modelview_matrix_with_animation = glm::rotate(modelview_matrix_with_animation, glm::radians(rotation_animation_status_.GetCurrentStatus().y), glm::vec3(0, 1, 0));
      modelview_matrix_with_animation = glm::rotate(modelview_matrix_with_animation, glm::radians(rotation_animation_status_.GetCurrentStatus().z), glm::vec3(0, 0, 1));

      modelview_matrix_with_animation = glm::translate(modelview_matrix_with_animation, translation_animation_status_.GetCurrentStatus());

      return modelview_matrix_with_animation;
    }

    void AlignPositionToOrigin() {

      if (!vertices_.size()) {
        return;
      }

      glm::vec3 adjust_amount(0.0, 0.0, 0.0);
      for (const auto &vertex : vertices_) {
        adjust_amount += vertex;
      }
      adjust_amount /= vertices_.size();

      for (auto &vertex : vertices_) {
        vertex -= adjust_amount;
      }
    }

    void Translate(const glm::vec3 &translation_vector) {
      local_modelview_matrix_ = glm::translate(local_modelview_matrix_, translation_vector);
    }

    void Rotate(const float rotation_angle, const glm::vec3 &rotation_axis) {
      local_modelview_matrix_ = glm::rotate(local_modelview_matrix_, rotation_angle, rotation_axis);
    }

    void SetColor(const glm::vec3 &color) {
      colors_ = std::vector<glm::vec3>(vertices_.size(), color);
    }

    void SetNormal() {
      normals_.resize(vertices_.size());
      for (size_t i = 0; i < vertices_.size(); i += 3) {
        normals_[i] = normals_[i + 1] = normals_[i + 2] = glm::normalize(glm::cross(vertices_[i + 1] - vertices_[i], vertices_[i + 2] - vertices_[i]));
      }
    }

    void Upload() {
      if (vertices_.size()) {
        glGenBuffers(1, &vbo_vertices_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
        glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(vertices_[0]), vertices_.data(), GL_STATIC_DRAW);
      }

      if (colors_.size()) {
        glGenBuffers(1, &vbo_colors_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_colors_);
        glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(colors_[0]), colors_.data(), GL_STATIC_DRAW);
      }

      if (normals_.size()) {
        glGenBuffers(1, &vbo_normals_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_);
        glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(normals_[0]), normals_.data(), GL_STATIC_DRAW);
      }

      if (uvs_.size()) {
        glGenBuffers(1, &vbo_uvs_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs_);
        glBufferData(GL_ARRAY_BUFFER, uvs_.size() * sizeof(uvs_[0]), uvs_.data(), GL_STATIC_DRAW);
      }
    }

    void Draw() {
      Draw(glm::mat4(1.0f));
    }

    void Draw(const glm::mat4 &parent_modelview_matrix) {

      if (vbo_vertices_) {
        glEnableVertexAttribArray(shader_attribute_vertex_position_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
        glVertexAttribPointer(shader_attribute_vertex_position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
      }

      if (vbo_colors_) {
        glEnableVertexAttribArray(shader_attribute_vertex_color_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_colors_);
        glVertexAttribPointer(shader_attribute_vertex_color_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
      }

      if (vbo_normals_) {
        glEnableVertexAttribArray(shader_attribute_vertex_normal_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_);
        glVertexAttribPointer(shader_attribute_vertex_normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
      }

      if (vbo_uvs_) {
        glEnableVertexAttribArray(shader_attribute_vertex_uv_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs_);
        glVertexAttribPointer(shader_attribute_vertex_uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);
      }

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_id_);
      glUniform1i(shader_uniform_texture_id, 0);

      glm::mat4 modelview_matrix = parent_modelview_matrix * GetModelviewMatrixWithAnimation();

      glUniformMatrix4fv(shader_uniform_modelview_matrix_id, 1, GL_FALSE, glm::value_ptr(modelview_matrix));

      glm::mat4 inverse_modelview_matrix = glm::inverse(modelview_matrix);
      glUniformMatrix3fv(shader_uniform_inverse_modelview_matrix_id, 1, GL_FALSE, glm::value_ptr(inverse_modelview_matrix));

      glm::mat3 transpose_inverse_modelview_matrix = glm::transpose(glm::inverse(glm::mat3(modelview_matrix)));
      glUniformMatrix3fv(shader_uniform_transpose_inverse_modelview_matrix_id, 1, GL_FALSE, glm::value_ptr(transpose_inverse_modelview_matrix));

      glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

      if (vbo_vertices_) {
        glDisableVertexAttribArray(shader_attribute_vertex_position_id);
      }

      if (vbo_colors_) {
        glDisableVertexAttribArray(shader_attribute_vertex_color_id);
      }

      if (vbo_normals_) {
        glDisableVertexAttribArray(shader_attribute_vertex_normal_id);
      }

      if (vbo_uvs_) {
        glDisableVertexAttribArray(shader_attribute_vertex_uv_id);
      }

    }

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> colors_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;

    GLuint texture_id_;

    AnimationStatus<glm::vec3> rotation_animation_status_;
    AnimationStatus<glm::vec3> translation_animation_status_;

  private:

    GLuint vbo_vertices_;
    GLuint vbo_normals_;
    GLuint vbo_colors_;
    GLuint vbo_uvs_;

    glm::mat4 local_modelview_matrix_;
  };

}