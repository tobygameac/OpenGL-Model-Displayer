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

namespace OpenGLModelDisplayer {

  // temp
  class GLMaterial {

  public:
    GLMaterial() : texture_ka_id(new GLuint(0)), texture_kd_id(new GLuint(0)), texture_ks_id(new GLuint(0)) {
    }

    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;

    std::shared_ptr<GLuint> texture_ka_id;
    std::shared_ptr<GLuint> texture_kd_id;
    std::shared_ptr<GLuint> texture_ks_id;

  };
  // temp

  class GLRobot : private GLHierarchicalModel {

  public:

    GLRobot();

    virtual void Update(const float delta_time);

    virtual void Draw();

    virtual void Draw(const glm::mat4 &root_modelview_matrix);

    bool BuildMeshFromObjFile(const std::string &file_path);

    void BuildSimpleMesh();

    void ClearAnimation();

    void WalkingMode();

    void RunningMode();

    void FlyingMode();

    void StupidMode();

  private:

    virtual void Upload();

    std::shared_ptr<HierarchicalMeshNode> torso_mesh_node_;

    std::shared_ptr<HierarchicalMeshNode> head_mesh_node_;

    std::shared_ptr<HierarchicalMeshNode> left_upper_arm_mesh_node_;
    std::shared_ptr<HierarchicalMeshNode> left_lower_arm_mesh_node_;

    std::shared_ptr<HierarchicalMeshNode> right_upper_arm_mesh_node_;
    std::shared_ptr<HierarchicalMeshNode> right_lower_arm_mesh_node_;

    std::shared_ptr<HierarchicalMeshNode> left_upper_leg_mesh_node_;
    std::shared_ptr<HierarchicalMeshNode> left_lower_leg_mesh_node_;

    std::shared_ptr<HierarchicalMeshNode> right_upper_leg_mesh_node_;
    std::shared_ptr<HierarchicalMeshNode> right_lower_leg_mesh_node_;
  };

  GLRobot::GLRobot() :
    GLHierarchicalModel(),

    torso_mesh_node_(new HierarchicalMeshNode()),
    head_mesh_node_(new HierarchicalMeshNode()),

    left_upper_arm_mesh_node_(new HierarchicalMeshNode()),
    left_lower_arm_mesh_node_(new HierarchicalMeshNode()),

    right_upper_arm_mesh_node_(new HierarchicalMeshNode()),
    right_lower_arm_mesh_node_(new HierarchicalMeshNode()),

    left_upper_leg_mesh_node_(new HierarchicalMeshNode()),
    left_lower_leg_mesh_node_(new HierarchicalMeshNode()),

    right_upper_leg_mesh_node_(new HierarchicalMeshNode()),
    right_lower_leg_mesh_node_(new HierarchicalMeshNode()) {
      root_mesh_node_->SetChildMeshNode(torso_mesh_node_);

      torso_mesh_node_->SetChildMeshNode(head_mesh_node_);

      head_mesh_node_->SetNextMeshNode(left_upper_arm_mesh_node_);
      left_upper_arm_mesh_node_->SetNextMeshNode(right_upper_arm_mesh_node_);
      right_upper_arm_mesh_node_->SetNextMeshNode(left_upper_leg_mesh_node_);
      left_upper_leg_mesh_node_->SetNextMeshNode(right_upper_leg_mesh_node_);

      left_upper_arm_mesh_node_->SetChildMeshNode(left_lower_arm_mesh_node_);
      right_upper_arm_mesh_node_->SetChildMeshNode(right_lower_arm_mesh_node_);

      left_upper_leg_mesh_node_->SetChildMeshNode(left_lower_leg_mesh_node_);
      right_upper_leg_mesh_node_->SetChildMeshNode(right_lower_leg_mesh_node_);

      all_mesh_nodes_.push_back(torso_mesh_node_);

      all_mesh_nodes_.push_back(head_mesh_node_);

      all_mesh_nodes_.push_back(left_upper_arm_mesh_node_);
      all_mesh_nodes_.push_back(left_lower_arm_mesh_node_);

      all_mesh_nodes_.push_back(right_upper_arm_mesh_node_);
      all_mesh_nodes_.push_back(right_lower_arm_mesh_node_);

      all_mesh_nodes_.push_back(left_upper_leg_mesh_node_);
      all_mesh_nodes_.push_back(left_lower_leg_mesh_node_);

      all_mesh_nodes_.push_back(right_upper_leg_mesh_node_);
      all_mesh_nodes_.push_back(right_lower_leg_mesh_node_);
  }

  void GLRobot::Update(const float delta_time) {
    GLHierarchicalModel::Update(delta_time);
  }

  void GLRobot::Draw() {
    GLHierarchicalModel::Draw();
  }

  void GLRobot::Draw(const glm::mat4 &root_modelview_matrix) {
    GLHierarchicalModel::Draw(root_modelview_matrix);
  }

  void GLRobot::ClearAnimation() {
    for (const auto &mesh_node : all_mesh_nodes_) {
      mesh_node->ClearAnimation();
    }
  }

  void GLRobot::WalkingMode() {
    ClearAnimation();

    torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 1, 0), 1 / 3.0));
    torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -1, 0), 2 / 3.0));
    torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 0), 1 / 3.0));

    head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 15, 0), 1));
    head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -30, 0), 2));
    head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 15, 0), 1));

    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(10, 0, 0), 1.0 / 4.5));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 2.0 / 4.5));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(10, 0, 0), 1.0 / 4.5));

    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-10, 0, 0), 1.0 / 4.5));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 2.0 / 4.5));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-10, 0, 0), 1.0 / 4.5));

    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-10, 0, 0), 1.0 / 4.5));
    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 2.0 / 4.5));
    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-10, 0, 0), 1.0 / 4.5));

    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(10, 0, 0), 1.0 / 4.5));
    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 2.0 / 4.5));
    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(10, 0, 0), 1.0 / 4.5));
  }

  void GLRobot::RunningMode() {
    ClearAnimation();

    torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 5, 0), 1 / 4.0));
    torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -10, 0), 2 / 4.0));
    torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 5, 0), 1 / 4.0));

    head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 15, 0), 1 / 4.0));
    head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -30, 0), 2 / 4.0));
    head_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 15, 0), 1 / 4.0));

    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 4.0));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-40, 0, 0), 2.0 / 4.0));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 4.0));

    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 4.0));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(40, 0, 0), 2.0 / 4.0));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 4.0));

    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 4.0));
    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(40, 0, 0), 2.0 / 4.0));
    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 4.0));

    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 4.0));
    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-40, 0, 0), 2.0 / 4.0));
    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 4.0));

    left_lower_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-5, 0, 0), 1 / 3.0));
    left_lower_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(10, 0, 0), 2 / 3.0));
    left_lower_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-5, 0, 0), 1 / 3.0));

    right_lower_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-5, 0, 0), 1 / 3.0));
    right_lower_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(10, 0, 0), 2 / 3.0));
    right_lower_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-5, 0, 0), 1 / 3.0));
  }

  void GLRobot::FlyingMode() {
    ClearAnimation();

    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 20), 1.0 / 4.0));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, -40), 2.0 / 4.0));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 20), 1.0 / 4.0));

    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, -20), 1.0 / 4.0));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, 40), 2.0 / 4.0));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0, -20), 1.0 / 4.0));

    torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0.2, 0), 1));
    torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -0.2, 0), 1));
  }

  void GLRobot::StupidMode() {
    ClearAnimation();

    torso_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(360, 0, 0), 2));

    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 4.0));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-40, 0, 0), 2.0 / 4.0));
    left_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(20, 0, 0), 1.0 / 4.0));

    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 4.0));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(40, 0, 0), 2.0 / 4.0));
    right_upper_arm_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-20, 0, 0), 1.0 / 4.0));

    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 1.0 / 2.0));
    left_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 2.0));

    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(-60, 0, 0), 1.0 / 2.0));
    right_upper_leg_mesh_node_->AddRotationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(60, 0, 0), 1.0 / 2.0));

    torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, 0.2, 0), 1.0 / 2.0));
    torso_mesh_node_->AddTranslationAnimationAction(AnimationAction<glm::vec3>(glm::vec3(0, -0.2, 0), 1.0 / 2.0));
  }

  bool GLRobot::BuildMeshFromObjFile(const std::string &file_path) {

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
            GLTexture::SetGLTexture(cv::imread(file_folder_path + texture_file_path), target_mesh_node->GetMeshTextureId());

            // temp
            if (material_name == "head") {
            } else {
              GLTexture::SetGLTexture(cv::imread(file_folder_path + texture_file_path), torso_mesh_node_->GetMeshTextureId());
              *left_upper_arm_mesh_node_->GetMeshTextureId() = *torso_mesh_node_->GetMeshTextureId();
              *right_upper_arm_mesh_node_->GetMeshTextureId() = *torso_mesh_node_->GetMeshTextureId();
              *right_lower_arm_mesh_node_->GetMeshTextureId() = *torso_mesh_node_->GetMeshTextureId();
              *left_upper_leg_mesh_node_->GetMeshTextureId() = *torso_mesh_node_->GetMeshTextureId();
              *left_lower_leg_mesh_node_->GetMeshTextureId() = *torso_mesh_node_->GetMeshTextureId();
              *right_upper_leg_mesh_node_->GetMeshTextureId() = *torso_mesh_node_->GetMeshTextureId();
              *right_lower_leg_mesh_node_->GetMeshTextureId() = *torso_mesh_node_->GetMeshTextureId();
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
        int face_vertex_count = 0;
        while (input_string_stream >> vertex_detail) {
          std::replace(vertex_detail.begin(), vertex_detail.end(), '/', ' ');
          std::istringstream vertex_detail_string_stream(vertex_detail);
          size_t vertex_index;
          vertex_detail_string_stream >> vertex_index;

          // temp
          if (!face_vertex_count && target_mesh_node != head_mesh_node_) {
            target_mesh_node = torso_mesh_node_;
            if (obj_file_vertices[vertex_index - 1].x < -0.46) {
              if (obj_file_vertices[vertex_index - 1].y < 0.8) {
                target_mesh_node = left_lower_arm_mesh_node_;
              } else {
                target_mesh_node = left_upper_arm_mesh_node_;
              }
            } else if (obj_file_vertices[vertex_index - 1].x > 0.46) {
              if (obj_file_vertices[vertex_index - 1].y < 0.8) {
                target_mesh_node = right_lower_arm_mesh_node_;
              } else {
                target_mesh_node = right_upper_arm_mesh_node_;
              }
            } else if (obj_file_vertices[vertex_index - 1].y < 0.6) {
              if (obj_file_vertices[vertex_index - 1].x < 0) {
                if (obj_file_vertices[vertex_index - 1].y < 0.3) {
                  target_mesh_node = left_lower_leg_mesh_node_;
                } else {
                  target_mesh_node = left_upper_leg_mesh_node_;
                }
              } else if (obj_file_vertices[vertex_index - 1].x > 0) {
                if (obj_file_vertices[vertex_index - 1].y < 0.3) {
                  target_mesh_node = right_lower_leg_mesh_node_;
                } else {
                  target_mesh_node = right_lower_leg_mesh_node_;
                }
              }

            }
          }
          // temp

          target_mesh_node->GetMeshVertices().push_back(obj_file_vertices[vertex_index - 1]);

          if (current_material_name != "") {
            target_mesh_node->GetMeshColors().push_back(materials_map[current_material_name].kd);
          }

          if (vertex_detail_string_stream) {
            size_t vt_index;
            vertex_detail_string_stream >> vt_index;
            target_mesh_node->GetMeshUvs().push_back(obj_file_uvs[vt_index - 1]);
          }

          if (vertex_detail_string_stream) {
            size_t vn_index;
            vertex_detail_string_stream >> vn_index;
            target_mesh_node->GetMeshNormals().push_back(obj_file_normals[vn_index - 1]);
          }
          ++face_vertex_count;
        }

        if (face_vertex_count == 3) { // Triangle face
        } else {
        }
      } else if (line_header == "vn") {
        glm::vec3 normal;
        input_string_stream >> normal.x >> normal.y >> normal.z;
        obj_file_normals.push_back(normal);
      } else if (line_header == "vt") {
        glm::vec2 uv;
        input_string_stream >> uv.x >> uv.y;
        if (uv.y < 1.0) {
          uv.y += 1.0;
        }
        obj_file_uvs.push_back(uv);
      } else if (line_header == "vp") {
      } else {
      }
    }

    // temp


    root_mesh_node_->AlignPositionToOrigin();

    //root_mesh_node_->CreateBoxMesh(5, 0.001f, 5, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    // temp

    Upload();

    return true;
  }

  void GLRobot::BuildSimpleMesh() {
    root_mesh_node_->CreateBoxMesh(20, 0.001f, 20, glm::vec3(0, -8, 0), glm::vec3(1, 1, 1));

    torso_mesh_node_->CreateBoxMesh(4, 6, 4, glm::vec3(0, 9, 0), glm::vec3(0, 1, 0));

    head_mesh_node_->CreateBoxMesh(3, 2.5, 3, glm::vec3(0, 4.25, 0), glm::vec3(1, 0, 0));

    left_upper_arm_mesh_node_->CreateBoxMesh(1.5, 2, 1.5, glm::vec3(-2.75, 1, 0), glm::vec3(0, 0, 1));

    right_upper_arm_mesh_node_->CreateBoxMesh(1.5, 2, 1.5, glm::vec3(2.75, 1, 0), glm::vec3(0, 0, 1));

    left_lower_arm_mesh_node_->CreateBoxMesh(1, 2, 1, glm::vec3(0, -2, 0), glm::vec3(1, 1, 0));

    right_lower_arm_mesh_node_->CreateBoxMesh(1, 2, 1, glm::vec3(0, -2, 0), glm::vec3(1, 1, 0));

    left_upper_leg_mesh_node_->CreateBoxMesh(2, 3, 2, glm::vec3(-1.5, -4.5, 0), glm::vec3(0, 0, 1));

    right_upper_leg_mesh_node_->CreateBoxMesh(2, 3, 2, glm::vec3(1.5, -4.5, 0), glm::vec3(0, 0, 1));

    left_lower_leg_mesh_node_->CreateBoxMesh(1.5f, 3, 1.5f, glm::vec3(0, -3, 0), glm::vec3(1, 1, 0));

    right_lower_leg_mesh_node_->CreateBoxMesh(1.5f, 3, 1.4f, glm::vec3(0, -3, 0), glm::vec3(1, 1, 0));
  }

  void GLRobot::Upload() {
    GLHierarchicalModel::Upload();
  }

}