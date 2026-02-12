cmake_minimum_required(VERSION 3.28)
project(SunnyLand)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})


find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

add_executable(SunnyLand src/main.cpp
        src/engine/core/game_app.cpp
        src/engine/core/game_app.h)

target_link_libraries(SunnyLand PRIVATE SDL3::SDL3 SDL3_mixer::SDL3_mixer SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf glm::glm spdlog::spdlog)