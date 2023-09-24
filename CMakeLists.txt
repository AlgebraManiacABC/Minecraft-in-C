cmake_minimum_required(VERSION 3.23)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(minecraft)

file(GLOB SOURCES src/*c)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/lib)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -lm -std=c11)

link_directories(${SDL2_LIBRARY_DIRS})
message("Link directory SDL2_LIBRARY_DIRS: ${SDL2_LIBRARY_DIRS}")
link_directories(${Vulkan_LIBRARY_DIRS})
message("Link directory Vulkan_LIBRARY_DIRS: ${Vulkan_LIBRARY_DIRS}")

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} SDL2)
#link_libraries(SDL2::SDL2)
#link_libraries(SDL2)
message("Linked SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} Vulkan)
#link_libraries(Vulkan::Vulkan)
#link_libraries(Vulkan)
message("Linked Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")

include_directories(${SDL2_INCLUDE_DIRS})
message("Included SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")

include_directories(${Vulkan_INCLUDE_DIRS})
message("Included Vulkan_INCLUDE_DIRS: ${Vulkan_INCLUDE_DIRS}")

#message("C Compiler Flags: ${CMAKE_C_FLAGS}")
#message("C Linker Flags: ${CMAKE_C_LINK_FLAGS}")