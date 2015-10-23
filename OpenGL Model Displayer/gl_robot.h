#pragma once

#include <memory>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "gl_mesh.h"

namespace OpenGLModelDisplayer {

  class GLRobot {

  public:
    GLRobot() :
      root_mesh_node_(new MeshTreeNode()),

      torso_mesh_node_(new MeshTreeNode()),
      head_mesh_node_(new MeshTreeNode()),

      left_upper_arm_mesh_node_(new MeshTreeNode()),
      left_lower_arm_mesh_node_(new MeshTreeNode()),

      right_upper_arm_mesh_node_(new MeshTreeNode()),
      right_lower_arm_mesh_node_(new MeshTreeNode()),

      left_upper_leg_mesh_node_(new MeshTreeNode()),
      left_lower_leg_mesh_node_(new MeshTreeNode()),

      right_upper_leg_mesh_node_(new MeshTreeNode()),
      right_lower_leg_mesh_node_(new MeshTreeNode())
    {
      root_mesh_node_->child_mesh_root_ = torso_mesh_node_;

      torso_mesh_node_->child_mesh_root_ = head_mesh_node_;

      head_mesh_node_->next_mesh_node_ = left_upper_arm_mesh_node_;
      left_upper_arm_mesh_node_->next_mesh_node_ = right_upper_arm_mesh_node_;
      right_upper_arm_mesh_node_->next_mesh_node_ = left_upper_leg_mesh_node_;
      left_upper_leg_mesh_node_->next_mesh_node_ = right_upper_leg_mesh_node_;

      left_upper_arm_mesh_node_->child_mesh_root_ = left_lower_arm_mesh_node_;
      right_upper_arm_mesh_node_->child_mesh_root_ = right_lower_arm_mesh_node_;

      left_upper_leg_mesh_node_->child_mesh_root_ = left_lower_leg_mesh_node_;
      right_upper_leg_mesh_node_->child_mesh_root_ = right_lower_leg_mesh_node_;
    }

    void BuildSimpleMesh() {
      // Root
      root_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(root_mesh_node_->mesh_, 15, 0.001, 15);
      root_mesh_node_->mesh_->FillColors(glm::vec3(1, 1, 1));
      root_mesh_node_->mesh_->Upload();

      // Torso
      torso_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(torso_mesh_node_->mesh_, 4);
      torso_mesh_node_->mesh_->Translate(glm::vec3(0, 6, 0));
      torso_mesh_node_->mesh_->FillColors(glm::vec3(0, 1, 0));
      torso_mesh_node_->mesh_->Upload();

      // Head
      head_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(head_mesh_node_->mesh_, 2);
      head_mesh_node_->mesh_->Translate(glm::vec3(0, 3, 0));
      head_mesh_node_->mesh_->FillColors(glm::vec3(1, 0, 0));
      head_mesh_node_->mesh_->Upload();

      // Left upper arm
      left_upper_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(left_upper_arm_mesh_node_->mesh_, 1.2);
      left_upper_arm_mesh_node_->mesh_->Translate(glm::vec3(-2.6, 0, 0));
      left_upper_arm_mesh_node_->mesh_->FillColors(glm::vec3(0, 0, 1));
      left_upper_arm_mesh_node_->mesh_->Upload();

      // Right upper arm
      right_upper_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(right_upper_arm_mesh_node_->mesh_, 1.2);
      right_upper_arm_mesh_node_->mesh_->Translate(glm::vec3(2.6, 0, 0));
      right_upper_arm_mesh_node_->mesh_->FillColors(glm::vec3(0, 0, 1));
      right_upper_arm_mesh_node_->mesh_->Upload();

      // Left lower arm
      left_lower_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(left_lower_arm_mesh_node_->mesh_, 1);
      left_lower_arm_mesh_node_->mesh_->Translate(glm::vec3(0, -1, 0));
      left_lower_arm_mesh_node_->mesh_->FillColors(glm::vec3(1, 1, 0));
      left_lower_arm_mesh_node_->mesh_->Upload();

      // Right lower arm
      right_lower_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(right_lower_arm_mesh_node_->mesh_, 1);
      right_lower_arm_mesh_node_->mesh_->Translate(glm::vec3(0, -1, 0));
      right_lower_arm_mesh_node_->mesh_->FillColors(glm::vec3(1, 1, 0));
      right_lower_arm_mesh_node_->mesh_->Upload();

      // Left upper leg
      left_upper_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(left_upper_leg_mesh_node_->mesh_, 2.0);
      left_upper_leg_mesh_node_->mesh_->Translate(glm::vec3(-1.5, -3, 0));
      left_upper_leg_mesh_node_->mesh_->FillColors(glm::vec3(0, 0, 1));
      left_upper_leg_mesh_node_->mesh_->Upload();

      // Right upper leg
      right_upper_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(right_upper_leg_mesh_node_->mesh_, 2.0);
      right_upper_leg_mesh_node_->mesh_->Translate(glm::vec3(1.5, -3, 0));
      right_upper_leg_mesh_node_->mesh_->FillColors(glm::vec3(0, 0, 1));
      right_upper_leg_mesh_node_->mesh_->Upload();

      // Left lower leg
      left_lower_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(left_lower_leg_mesh_node_->mesh_, 1.5);
      left_lower_leg_mesh_node_->mesh_->Translate(glm::vec3(0, -0.75, 0));
      left_lower_leg_mesh_node_->mesh_->FillColors(glm::vec3(1, 1, 0));
      left_lower_leg_mesh_node_->mesh_->Upload();

      // Right lower leg
      right_lower_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      CreateBoxMesh(right_lower_leg_mesh_node_->mesh_, 1.5);
      right_lower_leg_mesh_node_->mesh_->Translate(glm::vec3(0, -0.75, 0));
      right_lower_leg_mesh_node_->mesh_->FillColors(glm::vec3(1, 1, 0));
      right_lower_leg_mesh_node_->mesh_->Upload();
    }

    void Draw() {
      root_mesh_node_->Draw(glm::mat4(1.0));
    }

    void Draw(const glm::mat4 &root_modelview_matrix) {
      root_mesh_node_->Draw(root_modelview_matrix);
    }

    std::shared_ptr<MeshTreeNode> root_mesh_node_;

    std::shared_ptr<MeshTreeNode> torso_mesh_node_;

    std::shared_ptr<MeshTreeNode> head_mesh_node_;

    std::shared_ptr<MeshTreeNode> left_upper_arm_mesh_node_;
    std::shared_ptr<MeshTreeNode> left_lower_arm_mesh_node_;

    std::shared_ptr<MeshTreeNode> right_upper_arm_mesh_node_;
    std::shared_ptr<MeshTreeNode> right_lower_arm_mesh_node_;

    std::shared_ptr<MeshTreeNode> left_upper_leg_mesh_node_;
    std::shared_ptr<MeshTreeNode> left_lower_leg_mesh_node_;

    std::shared_ptr<MeshTreeNode> right_upper_leg_mesh_node_;
    std::shared_ptr<MeshTreeNode> right_lower_leg_mesh_node_;

  private:

    void CreateBoxMesh(std::shared_ptr<GLMesh> mesh, const float size) {
      CreateBoxMesh(mesh, glm::vec3(0, 0, 0), size, size, size);
    }

    void CreateBoxMesh(std::shared_ptr<GLMesh> mesh, const float width, const float length, const float height) {
      CreateBoxMesh(mesh, glm::vec3(0, 0, 0), width, length, height);
    }

    void CreateBoxMesh(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float size) {
      CreateBoxMesh(mesh, center, size, size, size);
    }

    void CreateBoxMesh(std::shared_ptr<GLMesh> mesh, const glm::vec3 center, const float width, const float length, const float height) {
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

  };

}