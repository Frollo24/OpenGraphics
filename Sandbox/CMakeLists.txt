cmake_minimum_required(VERSION 3.28)
project(Sandbox LANGUAGES C CXX)

set(TARGET Sandbox)
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.h")

add_executable(${TARGET} ${SRC_FILES})
target_include_directories(${TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/OpenEngine/src")

target_link_libraries(${TARGET} PUBLIC OpenEngine)

add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/Sandbox/assets/"
        "${CMAKE_BINARY_DIR}/Sandbox/assets/")
