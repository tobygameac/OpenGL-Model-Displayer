#pragma once

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <opencv\cv.hpp>

#include "gl_animation.h"
#include "gl_hierarchical_model.h"
#include "gl_mesh.h"
#include "gl_texture.h"

class GLRobot : private GLHierarchicalModel {

public:

  GLRobot();

  virtual void Update(const float delta_time);

  virtual void Draw(const GLShader &gl_shader);

  virtual void Draw(const GLShader &gl_shader, const glm::mat4 &root_modelview_matrix);

  bool BuildMeshFromOptimusPrimeObjFile(const std::string &file_path);

  void BuildSimpleMesh();

  void ClearAnimation();

  void WalkingMode();

  void RunningMode();

  void KongfuMode();

  void SpikeMode();

private:

  virtual void Upload();

  glm::vec3 moving_speed_;

  glm::vec3 moving_radius_;
  glm::vec3 current_degree_;

  glm::vec3 walking_speed_;
  glm::vec3 running_speed_;

  std::shared_ptr<HierarchicalMeshNode> torso_mesh_node_;

  std::shared_ptr<HierarchicalMeshNode> head_mesh_node_;

  std::shared_ptr<HierarchicalMeshNode> left_upper_arm_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> left_lower_arm_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> left_upper_arm_joint_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> left_lower_arm_joint_mesh_node_;

  std::shared_ptr<HierarchicalMeshNode> right_upper_arm_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> right_lower_arm_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> right_upper_arm_joint_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> right_lower_arm_joint_mesh_node_;

  std::shared_ptr<HierarchicalMeshNode> left_upper_leg_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> left_lower_leg_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> left_upper_leg_joint_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> left_lower_leg_joint_mesh_node_;

  std::shared_ptr<HierarchicalMeshNode> right_upper_leg_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> right_lower_leg_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> right_upper_leg_joint_mesh_node_;
  std::shared_ptr<HierarchicalMeshNode> right_lower_leg_joint_mesh_node_;
};