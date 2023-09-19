cmake_minimum_required(VERSION 3.20)

project(minecraft)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB SOURCES src/*c)
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -lm -std=c17)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})