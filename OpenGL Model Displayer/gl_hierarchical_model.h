#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "gl_mesh.h"
#include "gl_texture.h"

namespace OpenGLModelDisplayer {

  class HierarchicalMeshNode {

  public:
    HierarchicalMeshNode() : mesh_(new GLMesh()), next_mesh_node_(nullptr), child_mesh_node_(nullptr) {
    }

    HierarchicalMeshNode(std::shared_ptr<GLMesh> mesh) : mesh_(mesh), next_mesh_node_(nullptr), child_mesh_node_(nullptr) {
    }

    HierarchicalMeshNode(std::shared_ptr<GLMesh> mesh, std::shared_ptr<HierarchicalMeshNode> next_mesh_node, std::shared_ptr<HierarchicalMeshNode> child_mesh_root) : mesh_(mesh), next_mesh_node_(next_mesh_node), child_mesh_node_(child_mesh_root) {
    }

    void Clear() {

      mesh_->vertices_.clear();
      mesh_->colors_.clear();
      mesh_->normals_.clear();
      mesh_->uvs_.clear();

      next_mesh_node_ = NULL;
      child_mesh_node_ = NULL;

      Upload();
    }

    void AlignPositionToOrigin() {
      AlignPositionToOrigin(glm::vec3(0, 0, 0));
    }

    void AlignPositionToOrigin(const glm::vec3 parent_translation_vector) {
      if (mesh_ == nullptr) {
        return;
      }

      glm::vec3 adjust_amount = mesh_->AlignPositionToOrigin(parent_translation_vector);

      if (next_mesh_node_ != nullptr) {
        next_mesh_node_->AlignPositionToOrigin(parent_translation_vector);
      }

      if (child_mesh_node_ != nullptr) {
        glm::vec3 translation_vector = parent_translation_vector + adjust_amount;
        child_mesh_node_->AlignPositionToOrigin(translation_vector);
      }
    }

    void SetNormals() {
      if (mesh_ == nullptr) {
        return;
      }

      mesh_->SetNormal();

      if (next_mesh_node_ != nullptr) {
        next_mesh_node_->SetNormals();
      }

      if (child_mesh_node_ != nullptr) {
        child_mesh_node_->SetNormals();
      }
    }

    void TranslateMesh(const glm::vec3 &translation_vector) {
      mesh_->Translate(translation_vector);
    }

    void RotateMesh(const float rotation_angle, const glm::vec3 &rotation_axis) {
      mesh_->Rotate(rotation_angle, rotation_axis);
    }

    void CreateCubeMesh(const float width, const float length, const float height, const glm::vec3 &translation_vector) {
      mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(mesh_, width, length, height);
      mesh_->Translate(translation_vector);
      mesh_->SetRandomColors();
      mesh_->SetNormal();
      mesh_->Upload();
    }

    void CreateCubeMesh(const float width, const float length, const float height, const glm::vec3 &translation_vector, const glm::vec3 &color) {
      mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(mesh_, width, length, height);
      mesh_->Translate(translation_vector);
      mesh_->SetColor(color);
      mesh_->SetNormal();
      mesh_->Upload();
    }

    void CreateConeMesh(const float width, const float length, const float height, const glm::vec3 &translation_vector) {
      mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCone(mesh_, width, length, height);
      mesh_->Translate(translation_vector);
      mesh_->SetRandomColors();
      mesh_->SetNormal();
      mesh_->Upload();
    }

    void CreateConeMesh(const float width, const float length, const float height, const glm::vec3 &translation_vector, const glm::vec3 &color) {
      mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCone(mesh_, width, length, height);
      mesh_->Translate(translation_vector);
      mesh_->SetColor(color);
      mesh_->SetNormal();
      mesh_->Upload();
    }

    void AddMeshVertex(const glm::vec3 &mesh_vertex) {
      mesh_->vertices_.push_back(mesh_vertex);
    }

    void AddMeshColor(const glm::vec3 &mesh_color) {
      mesh_->colors_.push_back(mesh_color);;
    }

    void AddMeshNormal(const glm::vec3 &mesh_normal) {
      mesh_->normals_.push_back(mesh_normal);
    }

    void AddMeshUv(const glm::vec2 &mesh_uv) {
      mesh_->uvs_.push_back(mesh_uv);
    }

    void SetMeshTextureId(const GLuint texture_id) {
      mesh_->texture_id_ = texture_id;
    }

    void ClearAnimation() {
      mesh_->rotation_animation_ = mesh_->translation_animation_ = Animation<glm::vec3>();
    }

    void SetDefaultRotationAnimationStatus(const glm::vec3 &defalut_rotation_animation_status) {
      mesh_->rotation_animation_.SetDefaultAnimationStatus(defalut_rotation_animation_status);
    }

    void SetDefaultTranslationAnimationStatus(const glm::vec3 &defalut_translation_animation_status) {
      mesh_->translation_animation_.SetDefaultAnimationStatus(defalut_translation_animation_status);
    }

    void SetRotationAnimation(const Animation<glm::vec3> &rotation_animation) {
      mesh_->rotation_animation_ = rotation_animation;
    }

    void AddRotationAnimationAction(const AnimationAction<glm::vec3> &rotation_animation_action) {
      mesh_->rotation_animation_.AddAnimationAction(rotation_animation_action);
    }

    void SetTranslationAnimation(const Animation<glm::vec3> &translation_animation) {
      mesh_->translation_animation_ = translation_animation;
    }

    void AddTranslationAnimationAction(const AnimationAction<glm::vec3> &translation_animation_action) {
      mesh_->translation_animation_.AddAnimationAction(translation_animation_action);
    }

    void AddRotationAnimationDelay(const float time_delay) {
      mesh_->rotation_animation_.AddDelay(time_delay * 1000);
    }

    void AddTranslationAnimationDelay(const float time_delay) {
      mesh_->translation_animation_.AddDelay(time_delay * 1000);
    }

    void UpdateAnimationStatus(const float delta_time) {
      mesh_->UpdateAnimationStatus(delta_time);
    }

    void SetNextMeshNode(const std::shared_ptr<HierarchicalMeshNode> next_mesh_node) {
      next_mesh_node_ = next_mesh_node;
    }

    void SetChildMeshNode(const std::shared_ptr<HierarchicalMeshNode> child_mesh_node) {
      child_mesh_node_ = child_mesh_node;
    }

    void Upload() {
      if (mesh_ == nullptr) {
        return;
      }

      mesh_->Upload();

      if (next_mesh_node_ != nullptr) {
        next_mesh_node_->Upload();
      }

      if (child_mesh_node_ != nullptr) {
        child_mesh_node_->Upload();
      }
    }

    void Draw() {
      Draw(glm::mat4(1.0f));
    }

    void Draw(const glm::mat4 &parent_modelview_matrix) {
      if (mesh_ == nullptr) {
        return;
      }

      mesh_->Draw(parent_modelview_matrix);

      if (next_mesh_node_ != nullptr) {
        next_mesh_node_->Draw(parent_modelview_matrix);
      }

      if (child_mesh_node_ != nullptr) {
        glm::mat4 modelview_matrix = parent_modelview_matrix * mesh_->GetModelviewMatrixWithAnimation();
        child_mesh_node_->Draw(modelview_matrix);
      }
    }

  private:

    std::shared_ptr<GLMesh> mesh_;
    std::shared_ptr<HierarchicalMeshNode> next_mesh_node_;
    std::shared_ptr<HierarchicalMeshNode> child_mesh_node_;

  };

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

  class GLHierarchicalModel {

  protected:

    GLHierarchicalModel() : root_mesh_node_(new HierarchicalMeshNode()) {
      all_mesh_nodes_.push_back(root_mesh_node_);
    }

    virtual bool BuildMeshFromObjFile(const std::string &file_path) {
      root_mesh_node_->Clear();

      std::ifstream obj_file_stream(file_path);

      if (!obj_file_stream.is_open()) {
        std::cerr << "File " + file_path + " not found.\n";
        return false;
      }

      std::string file_folder_path = file_path.substr(0, file_path.find_last_of("/\\") + 1);

      std::vector<glm::vec3> obj_file_vertices;
      std::vector<glm::vec3> obj_file_normals;
      std::vector<glm::vec2> obj_file_uvs;

      std::map<std::string, GLMaterial> materials_map;
      std::map<std::string, std::shared_ptr<HierarchicalMeshNode> > mesh_node_map;
      mesh_node_map[""] = std::shared_ptr<HierarchicalMeshNode>(new HierarchicalMeshNode());

      std::shared_ptr<HierarchicalMeshNode> target_mesh_node = mesh_node_map[""];

      std::string current_material_name;

      bool need_to_calculate_normals_ = false;

      std::string line_buffer;

      while (std::getline(obj_file_stream, line_buffer)) {
        std::istringstream input_string_stream(line_buffer);
        std::string line_header;
        input_string_stream >> line_header;

        if (line_header == "#") {
          continue;
        }

        std::transform(line_header.begin(), line_header.end(), line_header.begin(), ::tolower);

        if (line_header == "mtllib") {
          std::string mtl_file_path;
          input_string_stream >> mtl_file_path;

          std::ifstream mtl_file_stream(file_folder_path + mtl_file_path);
          if (!mtl_file_stream.is_open()) {
            std::cerr << "File " + file_folder_path + mtl_file_path + " not found.\n";
            continue;
          }

          std::string material_name;
          while (std::getline(mtl_file_stream, line_buffer)) {
            input_string_stream = std::istringstream(line_buffer);
            input_string_stream >> line_header;
            std::transform(line_header.begin(), line_header.end(), line_header.begin(), ::tolower);
            if (line_header == "newmtl") {
              input_string_stream >> material_name;
              mesh_node_map[material_name] = std::shared_ptr<HierarchicalMeshNode>(new HierarchicalMeshNode());
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
              mesh_node_map[material_name]->SetMeshTextureId(texture_id);
            } else if (line_header == "map_ks") {
            }

          }

        } else if (line_header == "usemtl") {
          input_string_stream >> current_material_name;
          if (materials_map.find(current_material_name) == materials_map.end()) {
            current_material_name = "";
          }
          target_mesh_node = mesh_node_map[current_material_name];
        } else if (line_header == "o") {
        } else if (line_header == "g") {
        } else if (line_header == "s") {
        } else if (line_header == "v") {
          glm::vec3 vertex;
          input_string_stream >> vertex.x >> vertex.y >> vertex.z;
          obj_file_vertices.push_back(vertex);      
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
        } else if (line_header == "f") {
          std::string vertex_detail_string;

          std::vector<size_t> face_vertex_indices;
          std::vector<size_t> face_vt_indices;
          std::vector<size_t> face_vn_indices;

          while (input_string_stream >> vertex_detail_string) {

            size_t first_slash_position = vertex_detail_string.find('/');
            size_t second_slash_position = std::string::npos;

            if (first_slash_position != std::string::npos) {
              second_slash_position = vertex_detail_string.find('/', first_slash_position + 1);
            }

            size_t face_vertex_index = 0;
            for (size_t i = 0; vertex_detail_string[i] && i != first_slash_position; ++i) {
              if (std::isdigit(vertex_detail_string[i])) {
                face_vertex_index = face_vertex_index * 10 + vertex_detail_string[i] - '0';
              }
            }
            face_vertex_indices.push_back(face_vertex_index - 1);

            if (first_slash_position != std::string::npos) {
              size_t face_vt_index = 0;
              bool has_number = false;
              for (size_t i = first_slash_position + 1; vertex_detail_string[i] && i != second_slash_position; ++i) {
                if (std::isdigit(vertex_detail_string[i])) {
                  face_vt_index = face_vt_index * 10 + vertex_detail_string[i] - '0';
                  has_number = true;
                }
              }
              if (has_number) {
                face_vt_indices.push_back(face_vt_index - 1);
              }
            }

            if (second_slash_position != std::string::npos) {
              size_t face_vn_index = 0;
              bool has_number = false;
              for (size_t i = second_slash_position + 1; vertex_detail_string[i]; ++i) {
                if (std::isdigit(vertex_detail_string[i])) {
                  face_vn_index = face_vn_index * 10 + vertex_detail_string[i] - '0';
                  has_number = true;
                }
              }
              if (has_number) {
                face_vn_indices.push_back(face_vn_index - 1);
              }
            }
          }

          glm::vec3 face_color(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
          if (current_material_name != "") {
            //face_color = materials_map[current_material_name].kd;
          }

          for (size_t triangle_fan_v1_index = 1; triangle_fan_v1_index < face_vertex_indices.size(); ++triangle_fan_v1_index) {
            for (size_t triangle_fan_v2_index = triangle_fan_v1_index + 1; triangle_fan_v2_index < face_vertex_indices.size(); ++triangle_fan_v2_index) {
              target_mesh_node->AddMeshVertex(obj_file_vertices[face_vertex_indices[0]]);
              target_mesh_node->AddMeshVertex(obj_file_vertices[face_vertex_indices[triangle_fan_v1_index]]);
              target_mesh_node->AddMeshVertex(obj_file_vertices[face_vertex_indices[triangle_fan_v2_index]]);

              target_mesh_node->AddMeshColor(face_color);
              target_mesh_node->AddMeshColor(face_color);
              target_mesh_node->AddMeshColor(face_color);

              if (face_vt_indices.size() == face_vertex_indices.size()) {
                target_mesh_node->AddMeshUv(obj_file_uvs[face_vt_indices[0]]);
                target_mesh_node->AddMeshUv(obj_file_uvs[face_vt_indices[triangle_fan_v1_index]]);
                target_mesh_node->AddMeshUv(obj_file_uvs[face_vt_indices[triangle_fan_v2_index]]);
              } else {
              }

              if (face_vn_indices.size() == face_vertex_indices.size()) {
                target_mesh_node->AddMeshNormal(obj_file_normals[face_vn_indices[0]]);
                target_mesh_node->AddMeshNormal(obj_file_normals[face_vn_indices[triangle_fan_v1_index]]);
                target_mesh_node->AddMeshNormal(obj_file_normals[face_vn_indices[triangle_fan_v2_index]]);
              } else {
                need_to_calculate_normals_ = true;
              }
            }
          }
        } else {
        }
      }

      std::shared_ptr<HierarchicalMeshNode> previous_mesh_node = root_mesh_node_;

      for (const auto &mesh_node_key_value : mesh_node_map) {
        std::shared_ptr<HierarchicalMeshNode> current_mesh_node = mesh_node_key_value.second;
        all_mesh_nodes_.push_back(current_mesh_node);
        current_mesh_node->AlignPositionToOrigin();

        if (need_to_calculate_normals_) {
          current_mesh_node->SetNormals();
        }

        previous_mesh_node->SetNextMeshNode(current_mesh_node);

        previous_mesh_node = current_mesh_node;
      }

      Upload();

      return true;
    }

    virtual void Update(const float delta_time) {
      for (const auto &mesh_node : all_mesh_nodes_) {
        mesh_node->UpdateAnimationStatus(delta_time);
      }
    }

    virtual void Upload() {
      root_mesh_node_->Upload();
    }

    virtual void Draw() {
      root_mesh_node_->Draw(glm::mat4(1.0));
    }

    virtual void Draw(const glm::mat4 &root_modelview_matrix) {
      root_mesh_node_->Draw(root_modelview_matrix);
    }

    std::shared_ptr<HierarchicalMeshNode> root_mesh_node_;

    std::vector<std::shared_ptr<HierarchicalMeshNode> > all_mesh_nodes_;
  };
}