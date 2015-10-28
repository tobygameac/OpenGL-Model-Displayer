#pragma once

#include <memory>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "gl_mesh.h"

namespace OpenGLModelDisplayer {

  class HierarchicalMeshNode {

  public:
    HierarchicalMeshNode() : mesh_(new GLMesh()), next_mesh_node_(nullptr), child_mesh_node_(nullptr) {}

    HierarchicalMeshNode(std::shared_ptr<GLMesh> mesh) : mesh_(mesh), next_mesh_node_(nullptr), child_mesh_node_(nullptr) {}

    HierarchicalMeshNode(std::shared_ptr<GLMesh> mesh, std::shared_ptr<HierarchicalMeshNode> next_mesh_node, std::shared_ptr<HierarchicalMeshNode> child_mesh_root) : mesh_(mesh), next_mesh_node_(next_mesh_node), child_mesh_node_(child_mesh_root) {
    }

    void AdjustMeshPosition() {
      mesh_->AdjustPosition();
    }

    void TranslateMesh(const glm::vec3 &translation_vector) {
      mesh_->Translate(translation_vector);
    }

    void RotateMesh(const float rotation_degree, const glm::vec3 &rotation_vector) {
      mesh_->Rotate(rotation_degree, rotation_vector);
    }

    void CreateBoxMesh(const float width, const float length, const float height, const glm::vec3 &translation_vector, const glm::vec3 &color) {
      mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(mesh_, width, length, height);
      mesh_->Translate(translation_vector);
      mesh_->SetColor(color);
      mesh_->SetNormal();
      mesh_->Upload();
    }

    std::vector<glm::vec3> &GetMeshVertices() {
      return mesh_->vertices_;
    }

    std::vector<glm::vec3> &GetMeshColors() {
      return mesh_->colors_;
    }

    std::vector<glm::vec3> &GetMeshNormals() {
      return mesh_->normals_;
    }

    std::vector<glm::vec2> &GetMeshUvs() {
      return mesh_->uvs_;
    }

    GLuint *GetMeshTextureId() {
      return &(mesh_->texture_id_);
    }

    void SetNextMeshNode(std::shared_ptr<HierarchicalMeshNode> next_mesh_node) {
      next_mesh_node_ = next_mesh_node;
    }

    void SetChildMeshNode(std::shared_ptr<HierarchicalMeshNode> child_mesh_node) {
      child_mesh_node_ = child_mesh_node;
    }

    void Upload() {
      if (mesh_ == nullptr) {
        return;
      }

      mesh_->Upload();

      if (next_mesh_node_ != nullptr) {
        next_mesh_node_->Upload();
      }

      if (child_mesh_node_ != nullptr) {
        child_mesh_node_->Upload();
      }
    }

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

      if (child_mesh_node_ != nullptr) {
        glm::mat4 modelview_matrix = parent_modelview_matrix * mesh_->local_modelview_matrix_;
        child_mesh_node_->Draw(modelview_matrix);
      }
    }

  private:

    std::shared_ptr<GLMesh> mesh_;
    std::shared_ptr<HierarchicalMeshNode> next_mesh_node_;
    std::shared_ptr<HierarchicalMeshNode> child_mesh_node_;

  };

  class GLHierarchicalModel {

  public:

    GLHierarchicalModel() : root_mesh_node_(new HierarchicalMeshNode()) {}

    virtual void Update(const float delta_time) {
    }

    void Upload() {
      root_mesh_node_->Upload();
    }

    void Draw() {
      root_mesh_node_->Draw(glm::mat4(1.0));
    }

    void Draw(const glm::mat4 &root_modelview_matrix) {
      root_mesh_node_->Draw(root_modelview_matrix);
    }

    std::shared_ptr<HierarchicalMeshNode> root_mesh_node_;
  };
}