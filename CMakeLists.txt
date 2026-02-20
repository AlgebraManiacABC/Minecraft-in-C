cmake_minimum_required(VERSION 3.28)
project(Minecraft-in-C C)

set(CMAKE_C_STANDARD 11)

# Adding Raylib - https://github.com/SasLuca/raylib-cmake-template/blob/master/CMakeLists.txt
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "master"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME} main.c
        world.c
        world.h)

# Include
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
# Link
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
