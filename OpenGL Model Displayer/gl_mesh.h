#pragma once

#include <memory>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace OpenGLModelDisplayer {

  extern GLint shader_program_id;
  extern GLint shader_attribute_vertex_position_id;
  extern GLint shader_attribute_vertex_color_id;
  extern GLint shader_attribute_vertex_normal_id;
  extern GLint shader_uniform_model_matrix_id;
  extern GLint shader_uniform_view_matrix_id;
  extern GLint shader_uniform_projection_matrix_id;

  class GLMesh {

  public:

    GLMesh() : vbo_vertices_(0), vbo_colors_(0), vbo_normals_(0), local_modelview_matrix_(glm::mat4(1.0)) {
    }

    void Translate(const glm::vec3 &translation_vector) {
      local_modelview_matrix_ = glm::translate(local_modelview_matrix_, translation_vector);
    }

    void Rotate(const float rotation_degree, const glm::vec3 &rotation_vector) {
      local_modelview_matrix_ = glm::rotate(local_modelview_matrix_, rotation_degree, rotation_vector);
    }

    void FillColors(const glm::vec3 &color) {
      colors_ = std::vector<glm::vec3>(vertices_.size(), color);
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

      glm::mat4 modelview_matrix = parent_modelview_matrix * local_modelview_matrix_;

      glUniformMatrix4fv(shader_uniform_model_matrix_id, 1, GL_FALSE, glm::value_ptr(modelview_matrix));

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

    }

    ~GLMesh() {
      if (vbo_vertices_) {
        //glDeleteBuffers(1, &vbo_vertices_);
      }

      if (vbo_normals_) {
        //glDeleteBuffers(1, &vbo_normals_);
      }
    }

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> colors_;
    std::vector<glm::vec3> normals_;

    glm::mat4 local_modelview_matrix_;

  private:

    GLuint vbo_vertices_;
    GLuint vbo_normals_;
    GLuint vbo_colors_;
  };

  class MeshTreeNode {

  public:
    MeshTreeNode() : mesh_(new GLMesh()), next_mesh_node_(nullptr), child_mesh_root_(nullptr) {}

    MeshTreeNode(GLMesh *mesh) : mesh_(mesh), next_mesh_node_(nullptr), child_mesh_root_(nullptr) {}

    MeshTreeNode(GLMesh *mesh, MeshTreeNode *next_mesh_node, MeshTreeNode *child_mesh_root) : mesh_(mesh), next_mesh_node_(next_mesh_node), child_mesh_root_(child_mesh_root) {}

    void Draw() {
      Draw(glm::mat4(1.0f));
    }

    void Draw(const glm::mat4 &parent_modelview_matrix) {
      if (mesh_ == nullptr) {
        return;
      }

      mesh_->Draw(parent_modelview_matrix);

      if (next_mesh_node_ != nullptr) {
        next_mesh_node_->Draw(parent_modelview_matrix);
      }

      if (child_mesh_root_ != nullptr) {
        glm::mat4 modelview_matrix = parent_modelview_matrix * mesh_->local_modelview_matrix_;
        child_mesh_root_->Draw(modelview_matrix);
      }
    }

    std::shared_ptr<GLMesh> mesh_;
    std::shared_ptr<MeshTreeNode> next_mesh_node_;
    std::shared_ptr<MeshTreeNode> child_mesh_root_;
  };

}