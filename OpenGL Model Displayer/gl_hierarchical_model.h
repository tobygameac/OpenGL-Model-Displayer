#pragma once

#include <functional>
#include <memory>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "gl_mesh.h"

namespace OpenGLModelDisplayer {

  class HierarchicalMeshNode {

  public:
    HierarchicalMeshNode() : mesh_(new GLMesh()), next_mesh_node_(nullptr), child_mesh_node_(nullptr) {
    }

    HierarchicalMeshNode(std::shared_ptr<GLMesh> mesh) : mesh_(mesh), next_mesh_node_(nullptr), child_mesh_node_(nullptr) {
    }

    HierarchicalMeshNode(std::shared_ptr<GLMesh> mesh, std::shared_ptr<HierarchicalMeshNode> next_mesh_node, std::shared_ptr<HierarchicalMeshNode> child_mesh_root) : mesh_(mesh), next_mesh_node_(next_mesh_node), child_mesh_node_(child_mesh_root) {
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

    void TranslateMesh(const glm::vec3 &translation_vector) {
      mesh_->Translate(translation_vector);
    }

    void RotateMesh(const float rotation_angle, const glm::vec3 &rotation_axis) {
      mesh_->Rotate(rotation_angle, rotation_axis);
    }

    void CreateBoxMesh(const float width, const float length, const float height, const glm::vec3 &translation_vector, const glm::vec3 &color) {
      mesh_ = std::shared_ptr<GLMesh>(new GLMesh());
      GLMesh::AddCube(mesh_, width, length, height);
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

  class GLHierarchicalModel {

  protected:

    GLHierarchicalModel() : root_mesh_node_(new HierarchicalMeshNode()) {
      all_mesh_nodes_.push_back(root_mesh_node_);
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