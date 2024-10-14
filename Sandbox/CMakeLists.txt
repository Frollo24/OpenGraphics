cmake_minimum_required(VERSION 3.28)
project(Sandbox LANGUAGES C CXX)

set(TARGET Sandbox)
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.h")

add_executable(${TARGET} ${SRC_FILES})
target_include_directories(${TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/OpenEngine/src")

target_link_libraries(${TARGET} PUBLIC OpenEngine)
