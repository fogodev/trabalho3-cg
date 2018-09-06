cmake_minimum_required(VERSION 3.12)
project(Trab3)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp globals.h point2d.h polygonal.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

