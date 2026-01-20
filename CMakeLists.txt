cmake_minimum_required(VERSION 3.15)
project(dynsys)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(SDL_PIPEWIRE OFF CACHE BOOL "disable pipewire" FORCE)

include(FetchContent)

FetchContent_Declare(SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.16
)
FetchContent_Declare(SDL_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-3.2.4
)
FetchContent_Declare(SDL_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG release-3.2.2
)

FetchContent_MakeAvailable(SDL3 SDL_image SDL_ttf)

add_executable(dynsys
  src/dynamicalsystems.cpp
  src/attractors.cpp
  src/3DRlinalg.cpp
  src/attractorTypes.cpp
)

find_package(GSL REQUIRED)
find_package(Threads REQUIRED)

target_include_directories(dynsys PRIVATE SDL_image/include)
target_include_directories(dynsys PRIVATE SDL_ttf/include)

target_link_libraries(dynsys PRIVATE
  GSL::gsl
  SDL3::SDL3
  SDL3::Headers
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  Threads::Threads
)
