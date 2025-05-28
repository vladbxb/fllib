cmake_minimum_required(VERSION 3.10)
project(fllib VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Please create a separate build directory and run CMake from there.")
endif()

# Compile every source file in src/ into a static library
file(GLOB LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_library(fllib STATIC ${LIB_SOURCES})

target_include_directories(fllib PUBLIC
  "${CMAKE_SOURCE_DIR}/include"
)

# Create executables for every demo file
file(GLOB DEMO_SOURCES "${CMAKE_SOURCE_DIR}/demos/*_demo.cpp")
foreach(DEMO_SRC ${DEMO_SOURCES})
  get_filename_component(DEMO_NAME ${DEMO_SRC} NAME_WE)
  add_executable(${DEMO_NAME} ${DEMO_SRC})
  target_link_libraries(${DEMO_NAME} PRIVATE fllib)
endforeach()

add_custom_target(demos ALL
    DEPENDS ${DEMO_SOURCES}
    COMMENT "Building all demo executables..."
)
