cmake_minimum_required(VERSION 3.20)
project(glad)

set(TARGET glad)
set(PUBLIC_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE SRC_FILES "*.c" "*.h")

add_library(${TARGET} STATIC ${SRC_FILES})

target_include_directories(${TARGET} PUBLIC ${PUBLIC_INCLUDES})
