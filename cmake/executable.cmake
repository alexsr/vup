get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" project_name ${project_name})
project(${project_name})

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

add_definitions(-DGLEW_STATIC)
add_definitions(-DGLEW_NO_GLU)

add_executable(${project_name} ${SOURCES} ${HEADER})

target_link_libraries(
    ${project_name}
    PRIVATE ${ALL_LIBRARIES}
    PUBLIC ${GLFW3_LIBRARIES}
    PUBLIC ${GLFW_STATIC_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${OPENGL_LIBRARIES}
)
