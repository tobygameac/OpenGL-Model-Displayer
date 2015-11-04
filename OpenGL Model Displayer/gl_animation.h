#pragma once

namespace OpenGLModelDisplayer {

  template <class T>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

  class AnimationStatus {

  public:

    AnimationStatus() : upper_bound_(), lower_bound_(), current_status_(), animation_update_amount_() {
    }

    AnimationStatus(T &upper_bound, T &lower_bound, T &animation_update_amount) : 
      upper_bound_(upper_bound), lower_bound_(lower_bound), animation_update_amount_(animation_update_amount), current_status_() {
    }

    AnimationStatus(T &upper_bound, T &lower_bound, T &animation_update_amount, T &current_status) : 
      upper_bound_(upper_bound), lower_bound_(lower_bound), animation_update_amount_(animation_update_amount), current_status_(current_status) {
    }

    void Update(const float delta_time);

    AnimationStatus operator = (const AnimationStatus &other) {
      upper_bound_ = other.upper_bound_;
      lower_bound_ = other.lower_bound_;
      animation_update_amount_ = other.animation_update_amount_;
      current_status_ = other.current_status_;
      return *this;
    }

    const T &GetCurrentStatus() {
      return current_status_;
    }

  private:

    T upper_bound_;
    T lower_bound_;

    T animation_update_amount_;

    T current_status_;
  };


  void AnimationStatus<glm::vec3>::Update(const float delta_time) {
    if (current_status_.x < lower_bound_.x || current_status_.x > upper_bound_.x) {
      animation_update_amount_.x *= -1;
    }

    if (current_status_.y < lower_bound_.y || current_status_.y > upper_bound_.y) {
      animation_update_amount_.y *= -1;
    }

    if (current_status_.z < lower_bound_.z || current_status_.z > upper_bound_.z) {
      animation_update_amount_.z *= -1;
    }

    glm::vec3 update_amount = animation_update_amount_ * delta_time * 0.001f;

    current_status_ += update_amount;
  }

}