#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <cmath>

template <typename T> class Vector3 {
public:
  T x, y, z;

  Vector3(T x_val = 0, T y_val = 0, T z_val = 0) : x(x_val), y(y_val), z(z_val) {}

  Vector3 operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  Vector3 operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  Vector3 operator*(T scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }

  Vector3<T> operator/(T scalar) const {
    if (scalar == 0) {
      throw std::invalid_argument("Division by zero.");
    }
    return Vector3<T>(x / scalar, y / scalar, z / scalar);
  }

  T& operator[](int index) {
    if (index < 0 || index >= 3) {
      throw std::out_of_range("Index out of range.");
    }

    if (index == 0) {
      return x;
    } else if (index == 1) {
      return y;
    } else {
      return z;
    }
  }

  const T& operator[](int index) const {
    if (index < 0 || index >= 3) {
      throw std::out_of_range("Index out of range.");
    }

    if (index == 0) {
      return x;
    } else if (index == 1) {
      return y;
    } else {
      return z;
    }
  }

  T dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }

  Vector3 cross(const Vector3& other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
  }

  T norm() const { return std::sqrt(x * x + y * y + z * z); }

  Vector3 normalized() const {
    T n = norm();
    if (n != 0) {
      return Vector3(x / n, y / n, z / n);
    }
    return *this;
  }
};

#endif