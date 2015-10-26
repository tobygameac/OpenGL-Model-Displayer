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
    HierarchicalMeshNode() : mesh_(new GLMesh()), next_mesh_node_(nullptr), child_mesh_root_(nullptr) {}

    HierarchicalMeshNode(GLMesh *mesh) : mesh_(mesh), next_mesh_node_(nullptr), child_mesh_root_(nullptr) {}

    HierarchicalMeshNode(GLMesh *mesh, HierarchicalMeshNode *next_mesh_node, HierarchicalMeshNode *child_mesh_root) : mesh_(mesh), next_mesh_node_(next_mesh_node), child_mesh_root_(child_mesh_root) {}

    void Draw() {
      Draw(glm::mat4(1.0f));
    }

    void Draw(const glm::mat4 &parent_model_matrix) {
      if (mesh_ == nullptr) {
        return;
      }

      mesh_->Draw(parent_model_matrix);

      if (next_mesh_node_ != nullptr) {
        next_mesh_node_->Draw(parent_model_matrix);
      }

      if (child_mesh_root_ != nullptr) {
        glm::mat4 model_matrix = parent_model_matrix * mesh_->local_model_matrix_;
        child_mesh_root_->Draw(model_matrix);
      }
    }

    std::shared_ptr<GLMesh> mesh_;
    std::shared_ptr<HierarchicalMeshNode> next_mesh_node_;
    std::shared_ptr<HierarchicalMeshNode> child_mesh_root_;
  };

  class GLHierarchicalModel {

  public:

    GLHierarchicalModel() : root_mesh_node_(new HierarchicalMeshNode()) {}

    void Draw() {
      root_mesh_node_->Draw(glm::mat4(1.0));
    }

    void Draw(const glm::mat4 &root_model_matrix) {
      root_mesh_node_->Draw(root_model_matrix);
    }

    std::shared_ptr<HierarchicalMeshNode> root_mesh_node_;
  };
}