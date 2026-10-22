cmake_minimum_required(VERSION 3.20)

project(comgrb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glm REQUIRED)
find_package(SDL3 REQUIRED)

add_executable(comgrb
        src/main.cpp
        src/Vertex.cpp
)

target_compile_definitions(comgrb PRIVATE
        _USE_MATH_DEFINES
        GLM_ENABLE_EXPERIMENTAL
)

target_compile_options(comgrb PRIVATE -fopenmp)
target_link_options(comgrb PRIVATE -fopenmp)

target_link_libraries(comgrb PRIVATE
        SDL3::SDL3
        glm::glm
)