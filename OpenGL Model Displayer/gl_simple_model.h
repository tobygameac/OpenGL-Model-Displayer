#pragma once

#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "gl_hierarchical_model.h"

namespace OpenGLModelDisplayer {

  class GLSimpleModel : private GLHierarchicalModel {

  public:

    GLSimpleModel();

    virtual bool BuildMeshFromObjFile(const std::string &file_path);

    virtual void Update(const float delta_time);

    virtual void Draw();

    virtual void Draw(const glm::mat4 &root_modelview_matrix);

  private:

    virtual void Upload();
  };

  GLSimpleModel::GLSimpleModel() : GLHierarchicalModel() {
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