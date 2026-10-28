# Uso local (Visual Studio / VS Code). A entrega usa o Makefile.
cmake_minimum_required(VERSION 3.20)
project(persistencia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(programa ${SOURCES})
target_include_directories(programa PRIVATE include)

if(MSVC)
  target_compile_options(programa PRIVATE /W4 /permissive- /utf-8)
else()
  target_compile_options(programa PRIVATE -Wall -Wextra -Wpedantic)
endif()
