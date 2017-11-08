get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

include_directories(
    ${OPENGL_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${ASSIMP_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${LIBRARIES_PATH}
)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)

add_definitions(-DSHADER_PATH="${SHADER_PATH}")
add_definitions(-DCOMPUTE_SHADER_PATH="${COMPUTE_SHADER_PATH}")
add_definitions(-DRESOURCES_PATH="${RESOURCES_PATH}")
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLFW_INCLUDE_GLCOREARB)
add_definitions(-DGLEW_NO_GLU)

add_library(${ProjectId} ${SOURCES} ${HEADER})

target_link_libraries(
    ${ProjectId}
    INTERFACE ${ALL_LIBRARIES}
    PUBLIC ${GLFW_STATIC_LIBRARIES}
    PUBLIC ${GLFW3_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${ASSIMP_LIBRARIES}
    stdc++fs
)
