cmake_minimum_required(VERSION 3.20)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(minecraft)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCES src/*c)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED GLEW_USE_STATIC_LIBS)
find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/lib)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIRS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -std=c11 -lm)

link_directories(lib)
#target_link_libraries(${PROJECT_NAME} m)
target_link_libraries(${PROJECT_NAME} ${GLEW_STATIC_LIBRARIES} m)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
