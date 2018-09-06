#include <GL/gl.h>     // The GL Header File
#include <GL/freeglut.h>   // The GL Utility Toolkit (Glut) Header

#include <cmath>
#include <vector>

#include "globals.h"
#include "point2d.h"

void mouse_points(int button, int state, int X, int Y) {
  // Respond to mouse button presses.
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Adding current mouse point to polygonal

    auto current_point = point<double>(X, Y).change_coordinates(width, height);

    switch (mode) {
      case Mode::drawing_polygon:
        if (!polygon.is_empty() && polygon.first() == current_point) {
          polygon.close_polygon();
          mode = Mode::checking_if_inside_polygon;
        } else
          polygon.add_point(current_point);
        break;

      case Mode::checking_if_inside_polygon:points.push_back(current_point);
        break;

      case Mode::marking_points:polygon.add_point(current_point);
        break;

      default:return;
    }

    glutPostRedisplay();
  }
}

void keyboard_cb(unsigned char key, int X, int Y) {
  switch (key) {
    case 27:             // ESCAPE key
      polygon = std::move(polygonal<double>()); // Cleaning polygon
      points = std::move(std::vector<point<double>>{}); // Cleaning points
      mode = Mode::marking_points; // Reset mode
      glutPostRedisplay();
      break;

      //    case 's':polygon.smooth_edges(); // Smoothing
      //      glutPostRedisplay();
      //      break;
    case 'c':
      if (mode != Mode::convex_hull) {
        mode = Mode::convex_hull;
        polygon.convex_hull_jarvis();
      }

      glutPostRedisplay();
      break;
  }
}

void display_points() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glPointSize(8.0);

  glLineWidth(4);

  switch (mode) {
    case Mode::drawing_polygon:
      glBegin(GL_LINE_STRIP);
      for (const auto& point: polygon) {
        glVertex2d(point.x, point.y); // Drawing lines between points
      }

      glEnd();

      glBegin(GL_POINTS);

      for (const auto& point: points) {
        if (polygon.is_inside(point, outsider)) // Draw a green point if is inside the polygon
          glColor3f(0.0, 1.0, 0.0);
        else // Draw a red point if it's outside
          glColor3f(1.0, 0.0, 0.0);
        glVertex2d(point.x, point.y);
      }

      glEnd();

      break;

    case Mode::marking_points:
      glBegin(GL_POINTS);
      for (const auto& point: polygon)
        glVertex2d(point.x, point.y); // Drawing points
      glEnd();
      break;

    case Mode::convex_hull:
      glBegin(GL_LINE_STRIP);
      for (const auto& point: polygon) {
        glVertex2d(point.x, point.y); // Drawing convex polygon
      }
      glEnd();
      break;
  }

  glutSwapBuffers();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(width, height);
  glutCreateWindow("Convex Hull");
  glutKeyboardFunc(keyboard_cb);
  glutMouseFunc(mouse_points);

  glutDisplayFunc(display_points);
  glutMainLoop();

  return EXIT_SUCCESS;
}
