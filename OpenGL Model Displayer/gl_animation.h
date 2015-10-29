#pragma once

namespace OpenGLModelDisplayer {

  template <class T>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

  class AnimationStatus {

  public:

    AnimationStatus() : upper_bound_(), lower_bound_(), current_status_(), animation_update_amount_() {
    }

    AnimationStatus(T &upper_bound, T &lower_bound, T &current_status, T &animation_update_amount) : 
      upper_bound_(upper_bound), lower_bound_(lower_bound), current_status_(current_status), animation_update_amount_(animation_update_amount) {
    }

    T Update(const float delta_time);

    AnimationStatus operator = (const AnimationStatus &other) {
      upper_bound_ = other.upper_bound_;
      lower_bound_ = other.lower_bound_;
      current_status_ = other.current_status_;
      animation_update_amount_ = other.animation_update_amount_;
      return *this;
    }

  private:

    T upper_bound_;
    T lower_bound_;

    T current_status_;

    T animation_update_amount_;
  };

  glm::vec3 AnimationStatus<glm::vec3>::Update(const float delta_time) {
    glm::vec3 update_amount = animation_update_amount_ * delta_time * 0.001f;
    current_status_ += update_amount;

    if (current_status_.x > upper_bound_.x || current_status_.y > upper_bound_.y || current_status_.z > upper_bound_.z) {
      animation_update_amount_ *= -1;
    } else if (current_status_.x < lower_bound_.x || current_status_.y < lower_bound_.y || current_status_.z < lower_bound_.z) {
      animation_update_amount_ *= -1;
    }

    return update_amount;
  }

}