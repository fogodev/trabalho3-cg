#ifndef TRAB1_POINT2D_H
#define TRAB1_POINT2D_H

#include <cmath>
#include <functional>

template <typename T>
struct point {
  T x, y;

  point() = default;

  point(T x, T y) noexcept :
      x(x),
      y(y) {}

  // Formula to change coordinates for OpenGL
  point<T> change_coordinates(T dim_x, T dim_y) {
    return point((2. * x) / dim_x - 1., -(2. * y) / dim_y + 1.);
  }

  // Some operator overloads
  point<T>& operator+=(const point<T>& other) {
    x += other.x;
    y += other.y;

    return *this;
  }

  point<T>& operator-=(const point<T>& other) {
    x -= other.x;
    y -= other.y;

    return *this;
  }

  point<T>& operator*=(double scalar) {
    x *= scalar;
    y *= scalar;

    return *this;
  }

  point<T>& operator/=(double scalar) {
    x /= scalar;
    y /= scalar;

    return *this;
  }

  // Adding points
  point<T> operator+(const point<T>& other) const {
    return point<T>(x + other.x, y + other.y);
  }

  // Subtracting points
  point<T> operator-(const point<T>& other) const {
    return point<T>(x - other.x, y - other.y);
  }

  // Multiply by scalar
  point<T> operator*(double scalar) const {
    return point<T>(x * scalar, y * scalar);
  }

  // Division by scalar
  point<T> operator/(double scalar) const {
    return point<T>(x / scalar, y / scalar);
  }

  // Check if 2 points are close enough to be considered equals
  bool operator!=(const point<T>& other) const {
    return std::abs(x - other.x) > 0.0001 && std::abs(y - other.y) > 0.0001;
  }

  bool operator==(const point<T>& other) const {
    return !(*this != other);
  }

  // Dor product
  T dot(const point<T>& other) const {
    return x * other.x + y * other.y;
  }

  // Vector's norm
  T norm() const {
    return std::sqrt(x * x + y * y);
  }

  // Cross product
  T cross(const point<T>& other) const {
    return x * other.y - y * other.x;
  }

  // Angle between 2 points
  double theta(const point<T>& other) const {
    return std::acos(this->dot(other) / (this->norm() * other.norm()));
  }

  // Pseudo angle between this point and x axis (î vector)
  double pseudo_angle() const {
    bool diag = x > y;
    bool adiag = x > -y;

    double pseudo_angle = !adiag ? 4.0 : 0.0;

    if (y < 0.0001)
      return pseudo_angle;
    if (diag ^ adiag)
      pseudo_angle += 2 - x / y;
    else
      pseudo_angle += y / x;

    return pseudo_angle;
  }

  // Orientation between 3 points (p, q, r)
  // returns 0 if p, q and r are colinear
  // returns 1 if clockwise
  // retunts 2 if counterclockwise
  static int orientation(const point<T>& p, const point<T>& q, const point<T>& r) {
    T value = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (value == 0) return 0;
    return (value > 0) ? 1 : 2;
  }

  // Rotation matrix
  void rotate(T theta) {
    T x_rot = x * cos(theta) + y * sin(theta);
    T y_rot = -x * sin(theta) + y * cos(theta);

    x = x_rot;
    y = y_rot;
  }
};

namespace std {
template <> struct hash<point<double>> {

  typedef point<double> argument_type;
  typedef size_t result_type;
  result_type operator() (const argument_type& p) const {


    return std::hash<double>{}(p.x) ^ std::hash<double>{}(p.y);
  }
};
}

#endif //TRAB1_POINT2D_H
