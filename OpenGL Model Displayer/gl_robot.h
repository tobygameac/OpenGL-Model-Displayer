#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <opencv\cv.hpp>

#include "gl_hierarchical_model.h"
#include "gl_mesh.h"
#include "gl_texture.h"

namespace OpenGLModelDisplayer {

  class GLRobot : public GLHierarchicalModel {

  public:
    GLRobot() :
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
      right_lower_leg_mesh_node_(new HierarchicalMeshNode())
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

    bool BuildMeshFromObjFile(const std::string &file_path) {

      std::string file_folder_path = file_path.substr(0, file_path.find_last_of("/\\") + 1);

      std::ifstream obj_file_stream(file_path);
      if (!obj_file_stream.is_open()) {
        return false;
      }

      std::string line_buffer;

      std::vector<glm::vec3> temp_vertices;
      std::vector<glm::vec3> temp_normals;
      std::vector<glm::vec2> temp_uvs;

      glm::vec3 color(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);

      std::map<std::string, GLMaterial> materials_map;

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
              GLTexture::SetGLTexture(cv::imread(file_folder_path + texture_file_path), &root_mesh_node_->mesh_->texture_id_);
            } else if (line_header == "map_ks") {
            }

          }

        } else if (line_header == "usemtl") {
          std::string material_name;
          input_string_stream >> material_name;
          color.x = materials_map[material_name].kd.x;
          color.y = materials_map[material_name].kd.y;
          color.z = materials_map[material_name].kd.z;
        } else if (line_header == "o") {
          //temp_vertices.clear();
          continue;
        } else if (line_header == "s") {
          continue;
        } else if (line_header == "v") {
          glm::vec3 vertex;
          input_string_stream >> vertex.x >> vertex.y >> vertex.z;
          temp_vertices.push_back(vertex);
        } else if (line_header == "f") {
          std::string vertex_detail;
          while (input_string_stream >> vertex_detail) {
            std::replace(vertex_detail.begin(), vertex_detail.end(), '/', ' ');
            std::istringstream vertex_detail_string_stream(vertex_detail);
            size_t vertex_index;
            vertex_detail_string_stream >> vertex_index;
            root_mesh_node_->mesh_->vertices_.push_back(temp_vertices[vertex_index - 1]);
            root_mesh_node_->mesh_->colors_.push_back(color);
            if (vertex_detail_string_stream) {
              size_t vt_index;
              vertex_detail_string_stream >> vt_index;
              root_mesh_node_->mesh_->uvs_.push_back(temp_uvs[vt_index - 1]);
            }
            if (vertex_detail_string_stream) {
              size_t vn_index;
              vertex_detail_string_stream >> vn_index;
              root_mesh_node_->mesh_->normals_.push_back(temp_normals[vn_index - 1]);
            }
          }
        } else if (line_header == "vn") {
          glm::vec3 normal;
          input_string_stream >> normal.x >> normal.y >> normal.z;
          temp_normals.push_back(normal);
        } else if (line_header == "vt") {
          glm::vec2 uv;
          input_string_stream >> uv.x >> uv.y;
          temp_uvs.push_back(uv);
        } else if (line_header == "vp") {
          continue;
        } else {
          continue;
        }
      }

      root_mesh_node_->mesh_->Upload();

      return true;
    }

    void BuildSimpleMesh() {
      // Root
      root_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(root_mesh_node_->mesh_, 20, 0.001f, 20);
      root_mesh_node_->mesh_->Translate(glm::vec3(0, -8, 0));
      root_mesh_node_->mesh_->SetColor(glm::vec3(1, 1, 1));
      root_mesh_node_->mesh_->SetNormal();
      root_mesh_node_->mesh_->Upload();

      // Torso
      torso_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(torso_mesh_node_->mesh_, 4, 6, 4);
      torso_mesh_node_->mesh_->Translate(glm::vec3(0, 9, 0));
      torso_mesh_node_->mesh_->SetColor(glm::vec3(0, 1, 0));
      torso_mesh_node_->mesh_->SetNormal();
      torso_mesh_node_->mesh_->Upload();

      // Head
      head_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(head_mesh_node_->mesh_, 3, 2.5, 3);
      head_mesh_node_->mesh_->Translate(glm::vec3(0, 4.25, 0));
      head_mesh_node_->mesh_->SetColor(glm::vec3(1, 0, 0));
      head_mesh_node_->mesh_->SetNormal();
      head_mesh_node_->mesh_->Upload();

      // Left upper arm
      left_upper_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(left_upper_arm_mesh_node_->mesh_, 1.5, 2, 1.5);
      left_upper_arm_mesh_node_->mesh_->Translate(glm::vec3(-2.75, 1, 0));
      left_upper_arm_mesh_node_->mesh_->SetColor(glm::vec3(0, 0, 1));
      left_upper_arm_mesh_node_->mesh_->SetNormal();
      left_upper_arm_mesh_node_->mesh_->Upload();

      // Right upper arm
      right_upper_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(right_upper_arm_mesh_node_->mesh_, 1.5, 2, 1.5);
      right_upper_arm_mesh_node_->mesh_->Translate(glm::vec3(2.75, 1, 0));
      right_upper_arm_mesh_node_->mesh_->SetColor(glm::vec3(0, 0, 1));
      right_upper_arm_mesh_node_->mesh_->SetNormal();
      right_upper_arm_mesh_node_->mesh_->Upload();

      // Left lower arm
      left_lower_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(left_lower_arm_mesh_node_->mesh_, 1, 2, 1);
      left_lower_arm_mesh_node_->mesh_->Translate(glm::vec3(0, -2, 0));
      left_lower_arm_mesh_node_->mesh_->SetColor(glm::vec3(1, 1, 0));
      left_lower_arm_mesh_node_->mesh_->SetNormal();
      left_lower_arm_mesh_node_->mesh_->Upload();

      // Right lower arm
      right_lower_arm_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(right_lower_arm_mesh_node_->mesh_, 1, 2, 1);
      right_lower_arm_mesh_node_->mesh_->Translate(glm::vec3(0, -2, 0));
      right_lower_arm_mesh_node_->mesh_->SetColor(glm::vec3(1, 1, 0));
      right_lower_arm_mesh_node_->mesh_->SetNormal();
      right_lower_arm_mesh_node_->mesh_->Upload();

      // Left upper leg
      left_upper_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(left_upper_leg_mesh_node_->mesh_, 2, 3, 2);
      left_upper_leg_mesh_node_->mesh_->Translate(glm::vec3(-1.5, -4.5, 0));
      left_upper_leg_mesh_node_->mesh_->SetColor(glm::vec3(0, 0, 1));
      left_upper_leg_mesh_node_->mesh_->SetNormal();
      left_upper_leg_mesh_node_->mesh_->Upload();

      // Right upper leg
      right_upper_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(right_upper_leg_mesh_node_->mesh_, 2, 3, 2);
      right_upper_leg_mesh_node_->mesh_->Translate(glm::vec3(1.5, -4.5, 0));
      right_upper_leg_mesh_node_->mesh_->SetColor(glm::vec3(0, 0, 1));
      right_upper_leg_mesh_node_->mesh_->SetNormal();
      right_upper_leg_mesh_node_->mesh_->Upload();

      // Left lower leg
      left_lower_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(left_lower_leg_mesh_node_->mesh_, 1.5f, 3, 1.4f);
      left_lower_leg_mesh_node_->mesh_->Translate(glm::vec3(0, -3, 0));
      left_lower_leg_mesh_node_->mesh_->SetColor(glm::vec3(1, 1, 0));
      left_lower_leg_mesh_node_->mesh_->SetNormal();
      left_lower_leg_mesh_node_->mesh_->Upload();

      // Right lower leg
      right_lower_leg_mesh_node_->mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(right_lower_leg_mesh_node_->mesh_, 1.5, 3, 1.5);
      right_lower_leg_mesh_node_->mesh_->Translate(glm::vec3(0, -3, 0));
      right_lower_leg_mesh_node_->mesh_->SetColor(glm::vec3(1, 1, 0));
      right_lower_leg_mesh_node_->mesh_->SetNormal();
      right_lower_leg_mesh_node_->mesh_->Upload();
    }

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

}