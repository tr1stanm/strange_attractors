cmake_minimum_required(VERSION 3.7)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
#this needs to be set before the project is built and files are linked etc.

project(dynsys)

add_executable(dynsys src/dynamicalsystems.cpp src/attractors.cpp src/3DRlinalg.cpp src/gfx.cpp)

#these commands allow the lsp to use autofill for external frameworks
find_package(GSL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_IMAGE REQUIRED)

target_link_libraries(dynsys PRIVATE GSL::gsl SDL3::SDL3 SDL3::Headers SDL3_image::SDL3_image)
