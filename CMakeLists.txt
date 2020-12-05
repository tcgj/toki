cmake_minimum_required(VERSION 3.13)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set project name
project(Toki)

# add sanitizers to module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers)

# check options
option(TK_USE_DOUBLE "Use double-precision floating point numbers." OFF)
if (TK_USE_DOUBLE)
    add_compile_definitions(TK_USE_DOUBLE)
endif ()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_definitions(TK_DEBUG_MODE)
endif ()

# add flags
set(CMAKE_CXX_FLAGS "-Wshadow-uncaptured-local")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# add sources
set(TK_CORE_HEADER
    src/core/aabb.hpp
    src/core/bxdf.hpp
    src/core/camera.hpp
    src/core/fresnel.hpp
    src/core/image.hpp
    src/core/integrator.hpp
    src/core/interaction.hpp
    src/core/light.hpp
    src/core/material.hpp
    src/core/parallel.hpp
    src/core/random.hpp
    src/core/ray.hpp
    src/core/region.hpp
    src/core/sampler.hpp
    src/core/scattering.hpp
    src/core/scene.hpp
    src/core/shape.hpp
    src/core/spectrum.hpp
    src/core/transform.hpp)

set(TK_CORE_SOURCE
    src/core/bxdf.cpp
    src/core/fresnel.cpp
    src/core/image.cpp
    src/core/integrator.cpp
    src/core/interaction.cpp
    src/core/light.cpp
    src/core/parallel.cpp
    src/core/random.cpp
    src/core/scattering.cpp
    src/core/scene.cpp
    src/core/shape.cpp
    src/core/transform.cpp)

file(GLOB TK_SOURCE
    src/accelerator/*
    src/bsdf/*
    src/camera/*
    src/image/*
    src/integrator/*
    src/light/*
    src/material/*
    src/math/*
    src/region/*
    src/sampler/*
    src/shape/*
    src/system/*
    src/util/*)

add_library(toki_lib STATIC
    ${TK_CORE_HEADER}
    ${TK_CORE_SOURCE}
    ${TK_SOURCE})
add_sanitizers(toki_lib)

# add executable
add_executable(toki_exe src/system/toki.cpp)
add_sanitizers(toki_exe)

# set include directories
target_include_directories(toki_lib PUBLIC include)
target_include_directories(toki_lib PUBLIC src)

set_target_properties(toki_exe PROPERTIES OUTPUT_NAME toki)
target_link_libraries(toki_exe PRIVATE toki_lib)
