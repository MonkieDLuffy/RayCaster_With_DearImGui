cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/pkgmgr/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(SimpleRayCaster VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)


find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(${PROJECT_NAME} 
  main.cpp
  render.cpp 
  render.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
