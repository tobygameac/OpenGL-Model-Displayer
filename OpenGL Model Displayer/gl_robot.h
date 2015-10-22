#pragma once

#include <memory>

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

      right_upper_leg_mesh_node_->child_mesh_root_ = left_lower_arm_mesh_node_;
      left_lower_arm_mesh_node_->child_mesh_root_ = right_lower_arm_mesh_node_;

      right_lower_arm_mesh_node_->child_mesh_root_ = left_lower_leg_mesh_node_;
      left_lower_leg_mesh_node_->child_mesh_root_ = right_lower_leg_mesh_node_;
    }

    void Draw(glm::mat4 mvp_matrix) {
      root_mesh_node_->Draw(mvp_matrix);
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
  };

}