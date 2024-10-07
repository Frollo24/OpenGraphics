cmake_minimum_required(VERSION 3.28)
project(OpenEngine LANGUAGES C CXX)

set(TARGET OpenEngine)
set(CMAKE_<CONFIG>_POSTFIX "")
file(GLOB_RECURSE SRC_FILES "src/*.h" "src/*.cpp")

add_library(${TARGET} SHARED ${SRC_FILES})
target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_compile_definitions(${TARGET} PUBLIC "OG_BUILD_SHARED")
