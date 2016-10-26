#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

template <class T>
class AnimationAction {

public:

  AnimationAction(T target_action, float time_need) : target_action_(target_action), time_need_(time_need * 1000) {
  }

  const T &TargetAction() {
    return target_action_;
  }

  float TimeNeed() {
    return time_need_;
  }

private:

  T target_action_;

  float time_need_;

};

template <class T>
class Animation {

public:

  Animation() : defalut_animation_status_(), previous_animation_status_(), animation_status_(),
    current_action_index_(0), time_used_(0), time_delayed_(0),
    is_loop_(true), is_playing_(true) {
  }

  Animation(bool is_loop) : Animation(), is_loop_(is_loop) {
  }

  Animation(const std::vector<AnimationAction<T> > &animation_action_list) : Animation(), animation_action_list_(animation_action_list) {
  }

  Animation(const std::vector<AnimationAction<T> > &animation_action_list, bool is_loop) : Animation(animation_action_list), is_loop_(is_loop) {
  }

  void SetDefaultAnimationStatus(const T &default_animation_status) {
    defalut_animation_status_ = default_animation_status;
  }

  void AddDelay(const float time_delayed) {
    time_delayed_ += time_delayed;
  }

  void AddAnimationAction(const AnimationAction<T> &animation_action) {
    animation_action_list_.push_back(animation_action);
  }

  void Update(const float delta_time) {
    if (!is_playing_) {
      return;
    }

    if (time_delayed_ > 0) {
      if (time_delayed_ >= delta_time) {
        time_delayed_ -= delta_time;
      } else {
        Update(delta_time - time_delayed_);
        time_delayed_ = 0;
      }
      return;
    }

    if (current_action_index_ >= 0 && current_action_index_ < animation_action_list_.size()) {
      time_used_ += delta_time;

      float time_need = animation_action_list_[current_action_index_].TimeNeed();

      float remaining_time = time_used_ - time_need;
      time_used_ = std::min(time_used_, time_need);

      float action_percent = time_used_ / time_need;

      animation_status_ = previous_animation_status_ + animation_action_list_[current_action_index_].TargetAction() * action_percent;

      if (remaining_time >= 0) {
        previous_animation_status_ = animation_status_;
        current_action_index_ = (current_action_index_ + 1) % animation_action_list_.size();
        time_used_ = 0;

        if (!current_action_index_) {
          if (is_loop_) {
            previous_animation_status_ = T();
          } else {
            is_playing_ = false;
          }
        }

        if (remaining_time > 0) {
          Update(remaining_time);
        }
      }
    }
  }

  const T AnimationStatus() {
    return defalut_animation_status_ + animation_status_;
  }

private:

  T defalut_animation_status_;
  T previous_animation_status_;
  T animation_status_;

  std::vector<AnimationAction<T> > animation_action_list_;
  size_t current_action_index_;
  float time_used_;

  float time_delayed_;

  bool is_playing_;
  bool is_loop_;

};