cmake_minimum_required(VERSION 3.20)

project(minecraft)

file(GLOB SOURCES src/*c)
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -lm -std=c17)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})