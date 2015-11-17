#pragma once

#include <memory>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "gl_hierarchical_model.h"
#include "gl_mesh.h"

namespace OpenGLModelDisplayer {

  class GLSimpleModel : private GLHierarchicalModel {

  public:

    GLSimpleModel();

    virtual bool BuildMeshFromObjFile(const std::string &file_path);

    virtual void Update(const float delta_time);

    virtual void Draw();

    virtual void Draw(const glm::mat4 &root_modelview_matrix);

    void BuildBasicDemoShape();

  private:

    virtual void Upload();
  };

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

  void GLSimpleModel::Draw() {
    GLHierarchicalModel::Draw();
  }

  void GLSimpleModel::Draw(const glm::mat4 &root_modelview_matrix) {
    GLHierarchicalModel::Draw(root_modelview_matrix);
  }

  void GLSimpleModel::Upload() {
    GLHierarchicalModel::Upload();
  }

}