#pragma once

#include <cmath>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class GLMath {

};

template <class T>
class Vector3 {

public:

  Vector3() : values_(std::vector<T>(3, 0)) {
  }

  Vector3(std::vector<T> values) : Vector3() {
    for (size_t i = 0; i < 3 && i < values.size(); ++i) {
      values_[i] = values[i];
    }
  }

  Vector3(T *values) : values_(std::vector<T>(values, values + 3)) {
  }

  Vector3(T a, T b, T c) : values_(std::vector<T>(3, a, b, c)) {
  }

  std::vector<T> values_;
};

template <class T>
class Vector4 {

public:

  Vector4() : values_(std::vector<T>(4, 0)) {
  }

  Vector4(Vector3<T> vector3) : values_(std::vector<T>(4, 0)) {
    for (size_t i = 0; i < 3; ++i) {
      values_[i] = vector3.values_[i];
    }
  }

  Vector4(std::vector<T> values) : Vector4() {
    for (size_t i = 0; i < 4 && i < values.size(); ++i) {
      values_[i] = values[i];
    }
  }

  Vector4(T *values) : values_(std::vector<T>(values, values + 4)) {
  }

  Vector4(T a, T b, T c, T d) : values_(std::vector<T>(4, a, b, c, d)) {
  }

  std::vector<T> values_;
};

template <class T>
class Matrix44 {

public:

  Matrix44() : values_(std::vector<std::vector<T> >(4, std::vector<T>(4, 0))) {
  }

  Matrix44(std::vector<T> values) : Matrix44() {
    for (size_t row = 0; row < 4; ++row) {
      for (size_t column = 0; column < 4; ++column) {
        if (row * 4 + column >= values.size()) {
          break;
        }
        values_[row][column] = values[row * 4 + column];
      }
    }
  }

  Matrix44(T *values) : Matrix44() {
    for (size_t row = 0; row < 4; ++row) {
      for (size_t column = 0; column < 4; ++column) {
        values_[row][column] = values[row * 4 + column];
      }
    }
  }

  Matrix44(T a, T b, T c, T d,
    T e, T f, T g, T h,
    T i, T j, T k, T l,
    T m, T n, T o, T p) : values_(
      std::vector<std::vector<T> >(4,
      std::vector<T>(4, a, b, c, d),
      std::vector<T>(4, e, f, g, h),
      std::vector<T>(4, i, j, k, l),
      std::vector<T>(4, m, n, o, p))) {
  }

  Matrix44<T> operator * (const T other_scale) const {
    Matrix44<T> result = *this;
    for (size_t row = 0; row < 4; ++row) {
      for (size_t column = 0; column < 4; ++column) {
        result[row][column] *= other_scale;
      }
    }
    return result;
  }

  Vector4<T> operator * (const Vector4<T> &other_vector4) const {
    Vector4<T> result;
    for (size_t row = 0; row < 4; ++row) {
      for (size_t column = 0; column < 4; ++column) {
        result.values_[row] += values_[row * 4 + column] * other_vector4.values_[row];
      }
    }
    return result;
  }

  Matrix44<T> operator * (const Matrix44<T> &other_matrix44) const {
    Matrix44<T> result;
    for (size_t row = 0; row < 4; ++row) {
      for (size_t column = 0; column < 4; ++column) {
        for (size_t k = 0; k < 4; ++k) {
          result.values_[column + row * 4] += other_matrix44.values_[column + k * 4] * values_[k + row * 4];
        }
      }
    }
    return result;
  }

  Matrix44 operator = (const Matrix44 &other_matrix44) const {
    values_ = other_matrix44.values_;
    return *this;
  }

  T GetDeterminantValue() {
    return
      _values[0][3] * _values[1][2] * _values[2][1] * _values[3][0] -
      _values[0][2] * _values[1][3] * _values[2][1] * _values[3][0] -
      _values[0][3] * _values[1][1] * _values[2][2] * _values[3][0] +
      _values[0][1] * _values[1][3] * _values[2][2] * _values[3][0] +
      _values[0][2] * _values[1][1] * _values[2][3] * _values[3][0] -
      _values[0][1] * _values[1][2] * _values[2][3] * _values[3][0] -
      _values[0][3] * _values[1][2] * _values[2][0] * _values[3][1] +
      _values[0][2] * _values[1][3] * _values[2][0] * _values[3][1] +
      _values[0][3] * _values[1][0] * _values[2][2] * _values[3][1] -
      _values[0][0] * _values[1][3] * _values[2][2] * _values[3][1] -
      _values[0][2] * _values[1][0] * _values[2][3] * _values[3][1] +
      _values[0][0] * _values[1][2] * _values[2][3] * _values[3][1] +
      _values[0][3] * _values[1][1] * _values[2][0] * _values[3][2] -
      _values[0][1] * _values[1][3] * _values[2][0] * _values[3][2] -
      _values[0][3] * _values[1][0] * _values[2][1] * _values[3][2] +
      _values[0][0] * _values[1][3] * _values[2][1] * _values[3][2] +
      _values[0][1] * _values[1][0] * _values[2][3] * _values[3][2] -
      _values[0][0] * _values[1][1] * _values[2][3] * _values[3][2] -
      _values[0][2] * _values[1][1] * _values[2][0] * _values[3][3] +
      _values[0][1] * _values[1][2] * _values[2][0] * _values[3][3] +
      _values[0][2] * _values[1][0] * _values[2][1] * _values[3][3] -
      _values[0][0] * _values[1][2] * _values[2][1] * _values[3][3] -
      _values[0][1] * _values[1][0] * _values[2][2] * _values[3][3] +
      _values[0][0] * _values[1][1] * _values[2][2] * _values[3][3];
  }

  T GetInversedMatrix() {
    T determinant = GetDeterminantValue();

    if (determinant == 0) {
      return *this;
    }

    Matrix44 result;

    result._values[0][0] = _values[1][2] * _values[2][3] * _values[3][1] - _values[1][3] * _values[2][2] * _values[3][1] + _values[1][3] * _values[2][1] * _values[3][2] - _values[1][1] * _values[2][3] * _values[3][2] - _values[1][2] * _values[2][1] * _values[3][3] + _values[1][1] * _values[2][2] * _values[3][3];
    result._values[0][1] = _values[0][3] * _values[2][2] * _values[3][1] - _values[0][2] * _values[2][3] * _values[3][1] - _values[0][3] * _values[2][1] * _values[3][2] + _values[0][1] * _values[2][3] * _values[3][2] + _values[0][2] * _values[2][1] * _values[3][3] - _values[0][1] * _values[2][2] * _values[3][3];
    result._values[0][2] = _values[0][2] * _values[1][3] * _values[3][1] - _values[0][3] * _values[1][2] * _values[3][1] + _values[0][3] * _values[1][1] * _values[3][2] - _values[0][1] * _values[1][3] * _values[3][2] - _values[0][2] * _values[1][1] * _values[3][3] + _values[0][1] * _values[1][2] * _values[3][3];
    result._values[0][3] = _values[0][3] * _values[1][2] * _values[2][1] - _values[0][2] * _values[1][3] * _values[2][1] - _values[0][3] * _values[1][1] * _values[2][2] + _values[0][1] * _values[1][3] * _values[2][2] + _values[0][2] * _values[1][1] * _values[2][3] - _values[0][1] * _values[1][2] * _values[2][3];
    result._values[1][0] = _values[1][3] * _values[2][2] * _values[3][0] - _values[1][2] * _values[2][3] * _values[3][0] - _values[1][3] * _values[2][0] * _values[3][2] + _values[1][0] * _values[2][3] * _values[3][2] + _values[1][2] * _values[2][0] * _values[3][3] - _values[1][0] * _values[2][2] * _values[3][3];
    result._values[1][1] = _values[0][2] * _values[2][3] * _values[3][0] - _values[0][3] * _values[2][2] * _values[3][0] + _values[0][3] * _values[2][0] * _values[3][2] - _values[0][0] * _values[2][3] * _values[3][2] - _values[0][2] * _values[2][0] * _values[3][3] + _values[0][0] * _values[2][2] * _values[3][3];
    result._values[1][2] = _values[0][3] * _values[1][2] * _values[3][0] - _values[0][2] * _values[1][3] * _values[3][0] - _values[0][3] * _values[1][0] * _values[3][2] + _values[0][0] * _values[1][3] * _values[3][2] + _values[0][2] * _values[1][0] * _values[3][3] - _values[0][0] * _values[1][2] * _values[3][3];
    result._values[1][3] = _values[0][2] * _values[1][3] * _values[2][0] - _values[0][3] * _values[1][2] * _values[2][0] + _values[0][3] * _values[1][0] * _values[2][2] - _values[0][0] * _values[1][3] * _values[2][2] - _values[0][2] * _values[1][0] * _values[2][3] + _values[0][0] * _values[1][2] * _values[2][3];
    result._values[2][0] = _values[1][1] * _values[2][3] * _values[3][0] - _values[1][3] * _values[2][1] * _values[3][0] + _values[1][3] * _values[2][0] * _values[3][1] - _values[1][0] * _values[2][3] * _values[3][1] - _values[1][1] * _values[2][0] * _values[3][3] + _values[1][0] * _values[2][1] * _values[3][3];
    result._values[2][1] = _values[0][3] * _values[2][1] * _values[3][0] - _values[0][1] * _values[2][3] * _values[3][0] - _values[0][3] * _values[2][0] * _values[3][1] + _values[0][0] * _values[2][3] * _values[3][1] + _values[0][1] * _values[2][0] * _values[3][3] - _values[0][0] * _values[2][1] * _values[3][3];
    result._values[2][2] = _values[0][1] * _values[1][3] * _values[3][0] - _values[0][3] * _values[1][1] * _values[3][0] + _values[0][3] * _values[1][0] * _values[3][1] - _values[0][0] * _values[1][3] * _values[3][1] - _values[0][1] * _values[1][0] * _values[3][3] + _values[0][0] * _values[1][1] * _values[3][3];
    result._values[2][3] = _values[0][3] * _values[1][1] * _values[2][0] - _values[0][1] * _values[1][3] * _values[2][0] - _values[0][3] * _values[1][0] * _values[2][1] + _values[0][0] * _values[1][3] * _values[2][1] + _values[0][1] * _values[1][0] * _values[2][3] - _values[0][0] * _values[1][1] * _values[2][3];
    result._values[3][0] = _values[1][2] * _values[2][1] * _values[3][0] - _values[1][1] * _values[2][2] * _values[3][0] - _values[1][2] * _values[2][0] * _values[3][1] + _values[1][0] * _values[2][2] * _values[3][1] + _values[1][1] * _values[2][0] * _values[3][2] - _values[1][0] * _values[2][1] * _values[3][2];
    result._values[3][1] = _values[0][1] * _values[2][2] * _values[3][0] - _values[0][2] * _values[2][1] * _values[3][0] + _values[0][2] * _values[2][0] * _values[3][1] - _values[0][0] * _values[2][2] * _values[3][1] - _values[0][1] * _values[2][0] * _values[3][2] + _values[0][0] * _values[2][1] * _values[3][2];
    result._values[3][2] = _values[0][2] * _values[1][1] * _values[3][0] - _values[0][1] * _values[1][2] * _values[3][0] - _values[0][2] * _values[1][0] * _values[3][1] + _values[0][0] * _values[1][2] * _values[3][1] + _values[0][1] * _values[1][0] * _values[3][2] - _values[0][0] * _values[1][1] * _values[3][2];
    result._values[3][3] = _values[0][1] * _values[1][2] * _values[2][0] - _values[0][2] * _values[1][1] * _values[2][0] + _values[0][2] * _values[1][0] * _values[2][1] - _values[0][0] * _values[1][2] * _values[2][1] - _values[0][1] * _values[1][0] * _values[2][2] + _values[0][0] * _values[1][1] * _values[2][2];

    return result * (1 / determinant);
  }

  Matrix44<T> GetTransposedMatrix() {
    return Matrix44(
      values_[0], values_[4], values_[8], values_[12],
      values_[1], values_[5], values_[9], values_[13],
      values_[2], values_[6], values_[10], values_[14],
      values_[3], values_[7], values_[11], values_[15]);
  }

  template <class T>
  static Matrix44<T> PerspectiveCameraMatrix(T fovy, T aspect_ratio, T z_near, T z_far) {

    T fovy_radian = fovy * (acos(-1) / 180.0);
    T range = (T)tan(fovy_radian / 2.0) * z_near;

    T scale_x = (2.0 * z_near) / (range * aspect_ratio + range * aspect_ratio);
    T scale_y = z_near / range;
    T scale_z = -(z_far + z_near) / (z_far - z_near);
    T position_z = -(2.0  * z_far * z_near) / (z_far - z_near);

    return Matrix44(
      scale_x, 0, 0, 0,
      0, scale_y, 0, 0,
      0, 0, scale_z, position_z,
      0, 0, -1, 0);
  }

  template <class T>
  inline static Matrix44<T> IdentityMatrix() {
    return Matrix44(
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1);
  }

  std::vector<std::vector<T> > values_;
};