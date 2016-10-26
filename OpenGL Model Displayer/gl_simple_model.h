#pragma once

#include <memory>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "gl_hierarchical_model.h"
#include "gl_mesh.h"

class GLSimpleModel : private GLHierarchicalModel {

public:

  GLSimpleModel();

  virtual bool BuildMeshFromObjFile(const std::string &file_path);

  virtual void Update(const float delta_time);

  virtual void Draw(const GLShader &gl_shader);

  virtual void Draw(const GLShader &gl_shader, const glm::mat4 &root_modelview_matrix);

  virtual void SetTextureID(GLuint texture_id);

  void BuildBasicDemoShape();

private:

  virtual void Upload();
};