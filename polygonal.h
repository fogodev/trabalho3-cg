#ifndef TRAB1_POLYGONAL_H
#define TRAB1_POLYGONAL_H

#include <vector>
#include <iostream>
#include <unordered_set>
#include "point2d.h"

template <typename T>
class polygonal {
  std::vector<point<T>> points;

 public:
  void add_point(const point<T>& point) {
    points.push_back(point);
  }

  // Repeating first point on last position to close the polygon
  void close_polygon() {
    points.push_back(points[0]);
  }

  bool is_empty() {
    return points.empty();
  }

  const point<T>& first() {
    return points.front();
  }

  auto size() const {
    return points.size();
  }

  bool is_inside(const point<T>& pt, const point<T>& outsider) {
    size_t edges_crossed = 0;

    auto outsiderpoint_line = pt - outsider; // Line between point to be tested and point outside
    for (size_t index = 0; index != points.size() - 1; ++index) {
      auto a_point = points[index];
      auto b_point = points[index + 1];

      auto ab_line = b_point - a_point; // Line between first and second vertices
      auto apoint_line = pt - a_point; // Line between point and first vertex
      auto aoutsider_line = outsider - a_point; // Line between outside point and first vertex

      auto outsidera_line = a_point - outsider; // Line between first vertex and outside point
      auto outsiderb_line = b_point - outsider; // Line between second vertex and outside point

      // (AB x AD) . (AB x AC) < 0  &&  (CD x CA) . (CD x CB) < 0
      if (ab_line.cross(apoint_line) * ab_line.cross(aoutsider_line) < 0 &&
          outsiderpoint_line.cross(outsidera_line) * outsiderpoint_line.cross(outsiderb_line) < 0)
        ++edges_crossed;
    }

    // If edges_crossed is even, the point is outside
    return bool(edges_crossed % 2);
  }

  size_t smallest_y_coordinate_point_index() const {
    size_t smallest_point_index = 0;

    for (size_t index = 1; index != points.size(); ++index)
      if (points[index].y < points[smallest_point_index].y)
        smallest_point_index = index;
      else if (points[index].y == points[smallest_point_index].y)
        if (points[index].x > points[smallest_point_index].y)
          smallest_point_index = index;

    return smallest_point_index;
  }

  void convex_hull_jarvis() {
    std::vector<point<T>> convex_hull;
    size_t first_point_index = this->smallest_y_coordinate_point_index();

    size_t current_point_index = first_point_index;
    size_t smallest_pseudo_angle_point_index = 0;
    do {
      convex_hull.push_back(points[current_point_index]);

      smallest_pseudo_angle_point_index = (current_point_index + 1) % points.size();
      for(size_t index = 0; index != points.size(); ++index)
        if(point<T>::orientation(points[current_point_index], points[index], points[smallest_pseudo_angle_point_index]) == 2)
          smallest_pseudo_angle_point_index = index;

      current_point_index = smallest_pseudo_angle_point_index;

    } while (  current_point_index != first_point_index);
    convex_hull.push_back(points[first_point_index]);

    this->points = convex_hull;
  }

  void smooth_edges() {
    std::vector<point<T>> new_points;

    // Pushing first point
    new_points.push_back(points[0]);

    for (size_t index = 0; index != points.size() - 1; ++index) {
      auto initial_point = points[index];
      auto final_point = points[index + 1];

      // First quarter point
      new_points.emplace_back(initial_point + (final_point - initial_point) / 4.0);

      // Mid point
      new_points.emplace_back(initial_point + (final_point - initial_point) / 2.0);

      // Last quarter point
      new_points.emplace_back(initial_point + ((final_point - initial_point) / 4.0) * 3);
    }

    // Pushing last point
    new_points.push_back(points[points.size() - 1]);

    // Moving new polygon
    points = std::move(new_points);
  }

  // Some C++ iterators bullshit
  auto begin() -> decltype(points.begin()) {
    return points.begin();
  }

  // Some C++ iterators bullshit
  auto end() -> decltype(points.end()) {
    return points.end();
  }
};


#endif //TRAB1_POLYGONAL_H
