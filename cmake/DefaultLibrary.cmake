cmake_policy(SET CMP0022 OLD)
if (${CMAKE_MAJOR_VERSION} GREATER 2)
    cmake_policy(SET CMP0038 OLD)
endif ()
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include_directories(
        ${OPENGL_INCLUDE_PATH}
        ${GLEW_INCLUDE_PATH}
        ${GLFW3_INCLUDE_PATH}
        ${GLM_INCLUDE_PATH}
        ${RAPIDJSON_INCLUDE_PATH}
        ${EXTERNAL_LIBRARY_PATHS}
        ${LIBRARIES_PATH}
)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)

add_definitions(-DSHADER_PATH="${SHADER_PATH}")
add_definitions(-DCOMPUTE_SHADER_PATH="${COMPUTE_SHADER_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DGLEW_STATIC)

add_library(${ProjectId} ${SOURCES} ${HEADER})

target_link_libraries(
        ${ProjectId}
        ${ALL_LIBRARIES}
        ${GLFW_STATIC_LIBRARIES}
        ${GLFW3_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${OPENGL_LIBRARIES}
)

set_target_properties(${ProjectId} PROPERTIES LINKER_LANGUAGE CXX)
