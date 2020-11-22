cmake_minimum_required(VERSION 3.17)
project(CADEulerOperation)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(CADEulerOperation
        src/common.h
        src/main.cpp
        src/B-rep.h src/B-rep.cpp
        src/EulerOperation.h src/EulerOperation.cpp
        src/ModelPainter.h src/ModelPainter.cpp src/common.cpp src/EulerTest.h)

#link_libraries(glfw glew)

target_link_libraries(CADEulerOperation glfw GLEW "-framework OpenGL" "-framework GLUT")