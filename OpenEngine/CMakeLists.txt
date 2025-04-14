cmake_minimum_required(VERSION 3.28)
project(OpenEngine LANGUAGES C CXX)

set(TARGET OpenEngine)
set(CMAKE_<CONFIG>_POSTFIX "")
file(GLOB_RECURSE SRC_FILES "src/*.h" "src/*.cpp" "src/*.inl"
        "vendor/stb_image/*")

add_library(${TARGET} SHARED ${SRC_FILES})
target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image")

target_precompile_headers(${TARGET} PUBLIC "src/ogpch.h")

#[[
# This no longer is necessary, since target_precompile_headers handles this automatically

if (MSVC)
    get_filename_component(PRECOMPILED_BASENAME "src/ogpch.h" NAME_WE)  # PRECOMPILED_BASENAME = ogpch
    set(PRECOMPILED_BINARY "${CMAKE_CURRENT_BINARY_DIR}/${PRECOMPILED_BASENAME}.pch")
    set_source_files_properties("src/ogpch.cpp" PROPERTIES
            COMPILE_FLAGS "/Yc\"ogpch.h\" /Fp\"${PRECOMPILED_BINARY}\""
            OBJECT_OUTPUTS "${PRECOMPILED_BINARY}")
    set_source_files_properties(SRC_FILES PROPERTIES
            COMPILE_FLAGS "/Yu\"ogpch.h\" /Fp\"${PRECOMPILED_BINARY}\""
            OBJECT_DEPENDS "${PRECOMPILED_BINARY}")
endif (MSVC)
]]

target_compile_definitions(${TARGET} PUBLIC "OG_BUILD_SHARED")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${TARGET} PUBLIC "OG_DEBUG")
else()
    target_compile_definitions(${TARGET} PUBLIC "OG_RELEASE")
endif()

add_subdirectory("vendor/GLFW")
add_subdirectory("vendor/glad")
add_subdirectory("vendor/glm")
add_subdirectory("vendor/assimp")
target_link_libraries(${TARGET} PUBLIC
    glfw
    glad
    glm
    assimp
)

add_custom_command(TARGET OpenEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_SOURCE_DIR}/OpenEngine/shaders/"
        "${CMAKE_BINARY_DIR}/OpenEngine/shaders/")

add_custom_command(TARGET OpenEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/OpenEngine/OpenEngine${CMAKE_DEBUG_POSTFIX}.dll"
        "${CMAKE_BINARY_DIR}/Sandbox/OpenEngine${CMAKE_DEBUG_POSTFIX}.dll")
