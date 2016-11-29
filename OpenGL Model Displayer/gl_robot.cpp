#pragma once

#include "gl_robot.h"

GLRobot::GLRobot() :
  GLHierarchicalModel(),

  torso_mesh_node_(new HierarchicalMeshNode()),
  head_mesh_node_(new HierarchicalMeshNode()),

  left_upper_arm_mesh_node_(new HierarchicalMeshNode()),
  left_lower_arm_mesh_node_(new HierarchicalMeshNode()),
  left_upper_arm_joint_mesh_node_(new HierarchicalMeshNode()),
  left_lower_arm_joint_mesh_node_(new HierarchicalMeshNode()),

  right_upper_arm_mesh_node_(new HierarchicalMeshNode()),
  right_lower_arm_mesh_node_(new HierarchicalMeshNode()),
  right_upper_arm_joint_mesh_node_(new HierarchicalMeshNode()),
  right_lower_arm_joint_mesh_node_(new HierarchicalMeshNode()),

  left_upper_leg_mesh_node_(new HierarchicalMeshNode()),
  left_lower_leg_mesh_node_(new HierarchicalMeshNode()),
  left_upper_leg_joint_mesh_node_(new HierarchicalMeshNode()),
  left_lower_leg_joint_mesh_node_(new HierarchicalMeshNode()),

  right_upper_leg_mesh_node_(new HierarchicalMeshNode()),
  right_lower_leg_mesh_node_(new HierarchicalMeshNode()),
  right_upper_leg_joint_mesh_node_(new HierarchicalMeshNode()),
  right_lower_leg_joint_mesh_node_(new HierarchicalMeshNode()) {
  root_mesh_node_->SetChildMeshNode(torso_mesh_node_);

  torso_mesh_node_->SetChildMeshNode(head_mesh_node_);

  head_mesh_node_->SetNextMeshNode(left_upper_arm_joint_mesh_node_);
  left_upper_arm_joint_mesh_node_->SetNextMeshNode(right_upper_arm_joint_mesh_node_);
  right_upper_arm_joint_mesh_node_->SetNextMeshNode(left_upper_leg_joint_mesh_node_);
  left_upper_leg_joint_mesh_node_->SetNextMeshNode(right_upper_leg_joint_mesh_node_);

  // Arm
  left_upper_arm_joint_mesh_node_->SetChildMeshNode(left_upper_arm_mesh_node_);
  left_upper_arm_mesh_node_->SetChildMeshNode(left_lower_arm_joint_mesh_node_);
  left_lower_arm_joint_mesh_node_->SetChildMeshNode(left_lower_arm_mesh_node_);

  right_upper_arm_joint_mesh_node_->SetChildMeshNode(right_upper_arm_mesh_node_);
  right_lower_arm_joint_mesh_node_->SetChildMeshNode(right_lower_arm_mesh_node_);
  right_upper_arm_mesh_node_->SetChildMeshNode(right_lower_arm_joint_mesh_node_);

  // Leg
  left_upper_leg_joint_mesh_node_->SetChildMeshNode(left_upper_leg_mesh_node_);
  left_upper_leg_mesh_node_->SetChildMeshNode(left_lower_leg_joint_mesh_node_);
  left_lower_leg_joint_mesh_node_->SetChildMeshNode(left_lower_leg_mesh_node_);

  right_upper_leg_joint_mesh_node_->SetChildMeshNode(right_upper_leg_mesh_node_);
  right_upper_leg_mesh_node_->SetChildMeshNode(right_lower_leg_joint_mesh_node_);
  right_lower_leg_joint_mesh_node_->SetChildMeshNode(right_lower_leg_mesh_node_);

  all_mesh_nodes_.push_back(torso_mesh_node_);

  all_mesh_nodes_.push_back(head_mesh_node_);

  all_mesh_nodes_.push_back(left_upper_arm_mesh_node_);
  all_mesh_nodes_.push_back(left_lower_arm_mesh_node_);
  all_mesh_nodes_.push_back(left_upper_arm_joint_mesh_node_);
  all_mesh_nodes_.push_back(left_lower_arm_joint_mesh_node_);

  all_mesh_nodes_.push_back(right_upper_arm_mesh_node_);
  all_mesh_nodes_.push_back(right_lower_arm_mesh_node_);
  all_mesh_nodes_.push_back(right_upper_arm_joint_mesh_node_);
  all_mesh_nodes_.push_back(right_lower_arm_joint_mesh_node_);

  all_mesh_nodes_.push_back(left_upper_leg_mesh_node_);
  all_mesh_nodes_.push_back(left_lower_leg_mesh_node_);
  all_mesh_nodes_.push_back(left_upper_leg_joint_mesh_node_);
  all_mesh_nodes_.push_back(left_lower_leg_joint_mesh_node_);

  all_mesh_nodes_.push_back(right_upper_leg_mesh_node_);
  all_mesh_nodes_.push_back(right_lower_leg_mesh_node_);
  all_mesh_nodes_.push_back(right_upper_leg_joint_mesh_node_);
  all_mesh_nodes_.push_back(right_lower_leg_joint_mesh_node_);

  moving_speed_ = glm::vec3(0, 0, 0);

  moving_radius_ = glm::vec3(3, 0, 0);
  current_degree_ = glm::vec3(0, 0, 0);

  walking_speed_ = glm::vec3(0, -0.01, 0);
  running_speed_ = glm::vec3(0, -0.05, 0);
}

void GLRobot::Update(const float delta_time) {
  current_degree_ = current_degree_ + moving_speed_ * delta_time;
  // Need to handle case > 360

  GLHierarchicalModel::Update(delta_time);
}

void GLRobot::Draw(const GLShader &gl_shader) {
  Draw(gl_shader, glm::mat4(1.0));
}

void GLRobot::Draw(const GLShader &gl_shader, const glm::mat4 &root_modelview_matrix) {
  glm::mat4 moving_matrix(1.0);

  moving_matrix = glm::rotate(moving_matrix, glm::radians(current_degree_.x), glm::vec3(1, 0, 0));
  moving_matrix = glm::rotate(moving_matrix, glm::radians(current_degree_.y), glm::vec3(0, 1, 0));
  moving_matrix = glm::rotate(moving_matrix, glm::radians(current_degree_.z), glm::vec3(0, 0, 1));

  moving_matrix = glm::translate(moving_matrix, moving_radius_);

  GLHierarchicalModel::Draw(gl_shader, root_modelview_matrix * moving_matrix);
}

void GLRobot::ClearAnimation() {
  moving_speed_ = glm::vec3(0);

  for (const auto &mesh_node : all_mesh_nodes_) {
    mesh_node->ClearAnimation();
  }
}

void GLRobot::WalkingMode() {
  ClearAnimation();

  const float TOTAL_TIMES = 1;

  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0.01, 0.02, 0), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-0.01, -0.02, 0), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-0.01, 0.02, 0), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0.01, -0.02, 0), 1.0 / 4.0 * TOTAL_TIMES));

  left_upper_arm_joint_mesh_node_->AddRotationAnimationDelay(1 / 2.0 * TOTAL_TIMES);
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  right_upper_leg_joint_mesh_node_->AddRotationAnimationDelay(1 / 2.0 * TOTAL_TIMES);
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  right_lower_leg_joint_mesh_node_->AddRotationAnimationDelay(1 / 2.0 * TOTAL_TIMES);
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  moving_speed_ = walking_speed_;
}

void GLRobot::RunningMode() {
  ClearAnimation();

  const float TOTAL_TIMES = 0.75;

  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(5, 5, 0), 1 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-5, -5, 0), 1 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(5, -5, 0), 1 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-5, 5, 0), 1 / 4.0 * TOTAL_TIMES));

  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0.05, 0.1, 0), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-0.05, -0.1, 0), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-0.05, 0.1, 0), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0.05, -0.1, 0), 1.0 / 4.0 * TOTAL_TIMES));

  head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 15, 0), 1 / 2.0 * TOTAL_TIMES));
  head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -30, 0), 1 * TOTAL_TIMES));
  head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 15, 0), 1 / 2.0 * TOTAL_TIMES));

  left_upper_arm_joint_mesh_node_->AddRotationAnimationDelay(1 / 2.0 * TOTAL_TIMES);
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  right_upper_leg_joint_mesh_node_->AddRotationAnimationDelay(1 / 2.0 * TOTAL_TIMES);
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  left_lower_arm_joint_mesh_node_->SetDefaultRotationAnimationStatus(glm::vec3(0, 0, 20));

  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 1 / 2.0 * TOTAL_TIMES));
  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 0), 1 / 2.0 * TOTAL_TIMES));

  right_lower_arm_joint_mesh_node_->SetDefaultRotationAnimationStatus(glm::vec3(0, 0, -20));

  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 0), 1 / 2.0 * TOTAL_TIMES));
  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 1 / 2.0 * TOTAL_TIMES));

  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(80, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-80, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  right_lower_leg_joint_mesh_node_->AddRotationAnimationDelay(1 / 2.0 * TOTAL_TIMES);
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(80, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-80, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  moving_speed_ = running_speed_;
}

void GLRobot::KongfuMode() {
  ClearAnimation();

  const float TOTAL_TIMES = 2.0;

  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 90, -90), 1.0 / 2.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 270, 90), 1.0 / 2.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 270, 90), 1.0 / 2.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 90, -90), 1.0 / 2.0 * TOTAL_TIMES));

  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, -30), 1.0 / 2.0 * TOTAL_TIMES));
  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 30), 1.0 / 2.0 * TOTAL_TIMES));
  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 1.0 * TOTAL_TIMES));

  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(120, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-120, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 3.0 / 2.0 * TOTAL_TIMES));

  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 1.0 * TOTAL_TIMES));
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 30), 1.0 / 2.0 * TOTAL_TIMES));
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, -30), 1.0 / 2.0 * TOTAL_TIMES));

  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 1.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(120, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-120, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 1.0 / 2.0 * TOTAL_TIMES));

  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 10), 1.0 / 4.0 * TOTAL_TIMES));
  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, -10), 3.0 / 4.0 * TOTAL_TIMES));

  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(90, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-120, 0, 0), 1.0 / 8.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 5.0 / 8.0 * TOTAL_TIMES));

  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, -10), 1.0 / 4.0 * TOTAL_TIMES));
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 10), 3.0 / 4.0 * TOTAL_TIMES));

  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(90, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-120, 0, 0), 1.0 / 8.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 5.0 / 8.0 * TOTAL_TIMES));

  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 2.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0.02, 0.8), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -0.02, -0.8), 3.0 / 4.0 * TOTAL_TIMES));
  //torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -0.02, 0), 1.0 / 2.0 * TOTAL_TIMES));

  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-120, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(120, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));

  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 2.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 30), 1.0 / 4.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, -30), 3.0 / 4.0 * TOTAL_TIMES));

  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 2.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, -30), 1.0 / 4.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 30), 3.0 / 4.0 * TOTAL_TIMES));

  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 2.0 * TOTAL_TIMES));
  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-135, 0, 90), 1 / 4.0 * TOTAL_TIMES));
  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(135, 0, -90), 3.0 / 4.0 * TOTAL_TIMES));

  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 2.0 * TOTAL_TIMES));
  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-135, 0, -90), 1 / 4.0 * TOTAL_TIMES));
  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(135, 0, 90), 3.0 / 4.0 * TOTAL_TIMES));
}

void GLRobot::SpikeMode() {
  ClearAnimation();

  const float TOTAL_TIMES = 2;

  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 30), 1.0 / 4.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(120, 0, -60), 1.0 / 4.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 30), 2.0 / 4.0 * TOTAL_TIMES));

  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, -30), 1.0 / 4.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(120, 0, 60), 1.0 / 4.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, -30), 2.0 / 4.0 * TOTAL_TIMES));

  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 60), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, -60), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 2.0 / 4.0 * TOTAL_TIMES));

  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, -60), 1.0 / 4.0 * TOTAL_TIMES));
  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 60), 1.0 / 4.0 * TOTAL_TIMES));
  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 2.0 / 4.0 * TOTAL_TIMES));

  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(45, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-45, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(45, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-45, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));

  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  left_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));

  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  right_upper_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(30, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));

  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));

  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  right_lower_leg_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));

  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-180, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(180, 0, 0), 3.0 / 4.0 * TOTAL_TIMES));
  left_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 0.0 / 4.0 * TOTAL_TIMES));

  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-180, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(180, 0, 0), 2.0 / 4.0 * TOTAL_TIMES));
  right_upper_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));

  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-120, 90, 30), 1.0 / 4.0 * TOTAL_TIMES));
  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(120, -90, -30), 3.0 / 4.0 * TOTAL_TIMES));
  left_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 0.0 / 4.0 * TOTAL_TIMES));

  right_lower_arm_joint_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 4.0 / 4.0 * TOTAL_TIMES));

  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(15, 0, 0), 2.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(15, 0, 0), 5.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-30, 0, 0), 1.0 / 4.0 * TOTAL_TIMES));

  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -0.2, 0), 2.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 1.2, 0.5), 5.0 / 4.0 * TOTAL_TIMES));
  torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -1.0, 0), 1.0 / 4.0 * TOTAL_TIMES));
}

bool GLRobot::BuildMeshFromOptimusPrimeObjFile(const std::string &file_path) {

  std::string file_folder_path = file_path.substr(0, file_path.find_last_of("/\\") + 1);

  std::ifstream obj_file_stream(file_path);
  if (!obj_file_stream.is_open()) {
    return false;
  }

  std::vector<glm::vec3> obj_file_vertices;
  std::vector<glm::vec3> obj_file_normals;
  std::vector<glm::vec2> obj_file_uvs;

  std::map<std::string, GLMaterial> materials_map;
  std::string current_material_name;

  std::string line_buffer;

  std::shared_ptr<HierarchicalMeshNode> target_mesh_node = root_mesh_node_;

  while (std::getline(obj_file_stream, line_buffer)) {
    std::transform(line_buffer.begin(), line_buffer.end(), line_buffer.begin(), ::tolower);
    std::istringstream input_string_stream(line_buffer);
    std::string line_header;
    input_string_stream >> line_header;

    if (line_header == "#") {
      continue;
    }

    if (line_header == "mtllib") {
      std::string mtl_file_path;
      input_string_stream >> mtl_file_path;

      std::ifstream mtl_file_stream(file_folder_path + mtl_file_path);
      if (!mtl_file_stream.is_open()) {
        continue;
      }

      std::string material_name;
      while (std::getline(mtl_file_stream, line_buffer)) {
        std::transform(line_buffer.begin(), line_buffer.end(), line_buffer.begin(), ::tolower);
        input_string_stream = std::istringstream(line_buffer);
        input_string_stream >> line_header;
        if (line_header == "newmtl") {
          input_string_stream >> material_name;

          // temp
          if (material_name == "head") {
            target_mesh_node = head_mesh_node_;
          } else {
            target_mesh_node = root_mesh_node_;
          }
          // temp

        } else if (line_header == "ka") {
          input_string_stream >> materials_map[material_name].ka.x >> materials_map[material_name].ka.y >> materials_map[material_name].ka.z;
        } else if (line_header == "kd") {
          input_string_stream >> materials_map[material_name].kd.x >> materials_map[material_name].kd.y >> materials_map[material_name].kd.z;
        } else if (line_header == "ks") {
          input_string_stream >> materials_map[material_name].ks.x >> materials_map[material_name].ks.y >> materials_map[material_name].ks.z;
        } else if (line_header == "map_ka") {
        } else if (line_header == "map_kd") {
          std::string texture_file_path;
          input_string_stream >> texture_file_path;
          GLuint texture_id;
          GLTexture::SetGLTexture(cv::imread(file_folder_path + texture_file_path), &texture_id);

          // temp
          if (material_name == "head") {
            head_mesh_node_->SetMeshTextureId(texture_id);
          } else {
            for (const auto &mesh_node : all_mesh_nodes_) {
              if (mesh_node != head_mesh_node_) {
                mesh_node->SetMeshTextureId(texture_id);
              }
            }
          }
          // temp

        } else if (line_header == "map_ks") {
        }

      }

    } else if (line_header == "usemtl") {
      input_string_stream >> current_material_name;

      // temp
      if (current_material_name == "head") {
        target_mesh_node = head_mesh_node_;
      } else {
        target_mesh_node = root_mesh_node_;
      }
      // temp

    } else if (line_header == "o") {
    } else if (line_header == "g") {
    } else if (line_header == "s") {
    } else if (line_header == "v") {
      glm::vec3 vertex;
      input_string_stream >> vertex.x >> vertex.y >> vertex.z;
      obj_file_vertices.push_back(vertex);
    } else if (line_header == "f") {
      std::string vertex_detail;

      std::vector<size_t> face_vertex_indices;
      std::vector<glm::vec3> face_vertex_kds;
      std::vector<size_t> face_vt_indices;
      std::vector<size_t> face_vn_indices;

      while (input_string_stream >> vertex_detail) {
        std::replace(vertex_detail.begin(), vertex_detail.end(), '/', ' ');
        std::istringstream vertex_detail_string_stream(vertex_detail);

        size_t face_vertex_index;
        vertex_detail_string_stream >> face_vertex_index;
        face_vertex_indices.push_back(face_vertex_index - 1);

        if (current_material_name != "") {
          face_vertex_kds.push_back(materials_map[current_material_name].kd);
        }

        if (vertex_detail_string_stream) {
          size_t face_vt_index;
          vertex_detail_string_stream >> face_vt_index;
          face_vt_indices.push_back(face_vt_index - 1);
        }

        if (vertex_detail_string_stream) {
          size_t face_vn_index;
          vertex_detail_string_stream >> face_vn_index;
          face_vn_indices.push_back(face_vn_index - 1);
        }
      }

      if (target_mesh_node != head_mesh_node_) {
        glm::vec3 face_center(0, 0, 0);
        for (const auto &face_vertex_index : face_vertex_indices) {
          face_center += obj_file_vertices[face_vertex_index] / (float)face_vertex_indices.size();
        }
        target_mesh_node = torso_mesh_node_;
        if (face_center.y > 0.3 && face_center.x < -0.3) {
          if (face_center.x > -0.31) {
            target_mesh_node = left_upper_arm_joint_mesh_node_;
          } else {
            if (face_center.y < 0.84) {
              target_mesh_node = left_lower_arm_mesh_node_;
            } else if (face_center.y < 0.85) {
              target_mesh_node = left_lower_arm_joint_mesh_node_;
            } else {
              target_mesh_node = left_upper_arm_mesh_node_;
            }
          }
        } else if (face_center.y > 0.3 && face_center.x > 0.36) {
          if (face_center.x < 0.37) {
            target_mesh_node = right_upper_arm_joint_mesh_node_;
          } else {
            if (face_center.y < 0.84) {
              target_mesh_node = right_lower_arm_mesh_node_;
            } else if (face_center.y < 0.85) {
              target_mesh_node = right_lower_arm_joint_mesh_node_;
            } else {
              target_mesh_node = right_upper_arm_mesh_node_;
            }
          }
        } else if (face_center.y < 0.6) {
          if (face_center.x < 0) {
            if (face_center.y < 0.045) {
              target_mesh_node = left_lower_leg_mesh_node_;
            } else if (face_center.y < 0.06) {
              target_mesh_node = left_lower_leg_joint_mesh_node_;
            } else if (face_center.y < 0.59) {
              target_mesh_node = left_upper_leg_mesh_node_;
            } else {
              target_mesh_node = left_upper_leg_joint_mesh_node_;
            }
          } else if (face_center.x > 0) {
            if (face_center.y < 0.045) {
              target_mesh_node = right_lower_leg_mesh_node_;
            } else if (face_center.y < 0.06) {
              target_mesh_node = right_lower_leg_joint_mesh_node_;
            } else if (face_center.y < 0.59) {
              target_mesh_node = right_upper_leg_mesh_node_;
            } else {
              target_mesh_node = right_upper_leg_joint_mesh_node_;
            }
          }
        }
      }

      for (const auto &face_vertex_index : face_vertex_indices) {
        target_mesh_node->AddMeshVertex(obj_file_vertices[face_vertex_index]);
      }

      for (const auto &face_vertex_kd : face_vertex_kds) {
        target_mesh_node->AddMeshColor(face_vertex_kd);
      }

      for (const auto &face_vt_index : face_vt_indices) {
        target_mesh_node->AddMeshUv(obj_file_uvs[face_vt_index]);
      }

      for (const auto &face_vn_index : face_vn_indices) {
        target_mesh_node->AddMeshNormal(obj_file_normals[face_vn_index]);
      }

    } else if (line_header == "vn") {
      glm::vec3 normal;
      input_string_stream >> normal.x >> normal.y >> normal.z;
      obj_file_normals.push_back(normal);
    } else if (line_header == "vt") {
      glm::vec2 uv;
      input_string_stream >> uv.x >> uv.y;
      if (uv.y < 0.0) {
        uv.y += 1.0;
      }
      obj_file_uvs.push_back(uv);
    } else if (line_header == "vp") {
    } else {
    }
  }

  root_mesh_node_->AlignPositionToOrigin();

  Upload();

  return true;
}

void GLRobot::BuildSimpleMesh() {
  // Need to add joint
  root_mesh_node_->CreateCubeMesh(20, 0.001f, 20, glm::vec3(0, -8, 0), glm::vec3(1, 1, 1));

  torso_mesh_node_->CreateCubeMesh(4, 6, 4, glm::vec3(0, 9, 0), glm::vec3(0, 1, 0));

  head_mesh_node_->CreateCubeMesh(3, 2.5, 3, glm::vec3(0, 4.25, 0), glm::vec3(1, 0, 0));

  left_upper_arm_mesh_node_->CreateCubeMesh(1.5, 2, 1.5, glm::vec3(-2.75, 1, 0), glm::vec3(0, 0, 1));

  right_upper_arm_mesh_node_->CreateCubeMesh(1.5, 2, 1.5, glm::vec3(2.75, 1, 0), glm::vec3(0, 0, 1));

  left_lower_arm_mesh_node_->CreateCubeMesh(1, 2, 1, glm::vec3(0, -2, 0), glm::vec3(1, 1, 0));

  right_lower_arm_mesh_node_->CreateCubeMesh(1, 2, 1, glm::vec3(0, -2, 0), glm::vec3(1, 1, 0));

  left_upper_leg_mesh_node_->CreateCubeMesh(2, 3, 2, glm::vec3(-1.5, -4.5, 0), glm::vec3(0, 0, 1));

  right_upper_leg_mesh_node_->CreateCubeMesh(2, 3, 2, glm::vec3(1.5, -4.5, 0), glm::vec3(0, 0, 1));

  left_lower_leg_mesh_node_->CreateCubeMesh(1.5f, 3, 1.5f, glm::vec3(0, -3, 0), glm::vec3(1, 1, 0));

  right_lower_leg_mesh_node_->CreateCubeMesh(1.5f, 3, 1.4f, glm::vec3(0, -3, 0), glm::vec3(1, 1, 0));
}

void GLRobot::Upload() {
  GLHierarchicalModel::Upload();
}

//namespace OldGLJustForDemo {
//
//  struct Polygon {
//    std::vector<glm::vec3> vertices;
//  };
//
//  struct TransformState {
//    glm::vec3 translation;
//    glm::vec3 rotation_degree;
//  };
//
//  class Robot {
//
//  public:
//
//    void Update(const float delta_time);
//
//    void DrawRobot();
//
//  private:
//
//    void DrawTorso();
//    void DrawHead();
//    void DrawUpperArm();
//    void DrawLowerArm();
//
//    TransformState torso_state_;
//    TransformState head_state_;
//    TransformState upper_arm_state_;
//    TransformState lower_arm_state_;
//
//    TransformState new_state;
//
//    std::vector<Polygon> torso_vertices_;
//    std::vector<Polygon> head_vertices_;
//    std::vector<Polygon> upper_arm_vertices_;
//    std::vector<Polygon> lower_arm_vertices_;
//  };
//
//  void Robot::Update(const float delta_time) {
//  }
//
//  void Robot::DrawRobot() {
//    glPushMatrix();
//
//    DrawTorso();
//
//    glPushMatrix();
//    glTranslatef(head_state_.translation.x, head_state_.translation.y, head_state_.translation.z);
//    glRotatef(head_state_.rotation_degree.x, 1, 0, 0);
//    glRotatef(head_state_.rotation_degree.x, 0, 1, 0);
//    glRotatef(head_state_.rotation_degree.x, 0, 0, 1);
//    DrawHead();
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslatef(upper_arm_state_.translation.x, upper_arm_state_.translation.y, upper_arm_state_.translation.z);
//    glRotatef(upper_arm_state_.rotation_degree.x, 1, 0, 0);
//    glRotatef(upper_arm_state_.rotation_degree.x, 0, 1, 0);
//    glRotatef(upper_arm_state_.rotation_degree.x, 0, 0, 1);
//    DrawUpperArm();
//
//    glPushMatrix();
//    glTranslatef(lower_arm_state_.translation.x, lower_arm_state_.translation.y, lower_arm_state_.translation.z);
//    glRotatef(lower_arm_state_.rotation_degree.x, 1, 0, 0);
//    glRotatef(lower_arm_state_.rotation_degree.x, 0, 1, 0);
//    glRotatef(lower_arm_state_.rotation_degree.x, 0, 0, 1);
//    DrawLowerArm();
//    glPopMatrix();
//
//    glPopMatrix();
//
//    glPopMatrix();
//  }
//
//  void Robot::DrawTorso() {
//    for (const Polygon &polygon : torso_vertices_) {
//      glBegin(GL_POLYGON);
//      for (const glm::vec3 &vertice : polygon.vertices) {
//        glVertex3d(vertice.x, vertice.y, vertice.z);
//      }
//      glEnd();
//    }
//  }
//
//  void Robot::DrawHead() {
//
//  }
//
//  void Robot::DrawUpperArm() {
//
//  }
//
//  void Robot::DrawLowerArm() {
//
//  }
//
//  class OpenGLRobotDisplayer {
//
//  public:
//    void InitializeOpenGL();
//
//    void Display();
//
//  private:
//
//    Robot robot_;
//    TransformState eye_state_;
//  };
//
//  void OpenGLRobotDisplayer::InitializeOpenGL() {
//
//  }
//
//  void OpenGLRobotDisplayer::Display() {
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//    gluLookAt(eye_state_.translation.x, eye_state_.translation.x, eye_state_.translation.x,
//      0, 0, 0,
//      0, 1, 0);
//
//    robot_.DrawRobot();
//  }
//}