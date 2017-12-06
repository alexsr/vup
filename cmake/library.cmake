get_filename_component(project_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" project_name ${project_name})
project(${project_name})

include_directories(
    ${OPENGL_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLFW3_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${STB_INCLUDE_PATH}
    ${ASSIMP_INCLUDE_PATH}
    ${EXTERNAL_LIBRARY_PATHS}
    ${LIBRARIES_PATH}
)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADER *.h)

add_definitions(-DGLEW_STATIC)
add_definitions(-DGLEW_NO_GLU)

add_library(${project_name} ${SOURCES} ${HEADER})

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    find_package(X11)
    set(X11_LIBRARIES ${X11_LIBRARIES} Xrandr Xxf86vm Xi pthread dl Xinerama Xcursor)
    set(experimental_filesys_library stdc++fs)
endif()

target_link_libraries(
    ${project_name}
    INTERFACE ${ALL_LIBRARIES}
    PUBLIC ${X11_LIBRARIES}
    PUBLIC ${GLFW3_LIBRARIES}
    PUBLIC ${GLFW_STATIC_LIBRARIES}
    PUBLIC ${GLEW_LIBRARIES}
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${ASSIMP_LIBRARIES}
    ${experimental_filesys_library}
)
