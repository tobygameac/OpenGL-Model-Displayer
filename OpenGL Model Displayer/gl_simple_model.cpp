#pragma once

#include "gl_simple_model.h"

GLSimpleModel::GLSimpleModel() : GLHierarchicalModel() {
}

void GLSimpleModel::BuildBasicDemoShape() {
  std::shared_ptr<HierarchicalMeshNode> cube_node(new HierarchicalMeshNode());
  cube_node->CreateCubeMesh(2, 2, 2, glm::vec3(-2, 0, 0));
  std::shared_ptr<HierarchicalMeshNode> cone_node(new HierarchicalMeshNode());
  cone_node->CreateConeMesh(2, 2, 2, glm::vec3(2, 0, 0));
  root_mesh_node_->SetChildMeshNode(cube_node);
  cube_node->SetNextMeshNode(cone_node);

  all_mesh_nodes_.push_back(cube_node);
  all_mesh_nodes_.push_back(cone_node);
}

bool GLSimpleModel::BuildMeshFromObjFile(const std::string &file_path) {
  return GLHierarchicalModel::BuildMeshFromObjFile(file_path);
}

void GLSimpleModel::Update(const float delta_time) {
  GLHierarchicalModel::Update(delta_time);
}

void GLSimpleModel::Draw(const GLShader &gl_shader) {
  GLHierarchicalModel::Draw(gl_shader);
}

void GLSimpleModel::Draw(const GLShader &gl_shader, const glm::mat4 &root_modelview_matrix) {
  GLHierarchicalModel::Draw(gl_shader, root_modelview_matrix);
}

void GLSimpleModel::Upload() {
  GLHierarchicalModel::Upload();
}

void GLSimpleModel::SetTextureID(GLuint texture_id) {
  GLHierarchicalModel::SetTextureID(texture_id);
}