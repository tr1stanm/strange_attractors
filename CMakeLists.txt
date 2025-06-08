cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
#this needs to be set before the project is built and files are linked etc.

project(dynsys)

add_executable(dynsys src/dynamicalsystems.cpp src/attractors.cpp src/3DRlinalg.cpp
		      src/attractorTypes.cpp)

target_include_directories(dynsys PRIVATE SDL_image/include)
target_include_directories(dynsys PRIVATE SDL_ttf/include)
add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)

#these commands allow the lsp to use autofill for external frameworks
find_package(GSL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(dynsys PRIVATE GSL::gsl SDL3::SDL3 SDL3::Headers Threads::Threads SDL_image SDL_ttf)
