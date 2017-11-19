get_filename_component(project_name "${CMAKE_SOURCE_DIR}" NAME)
project(${project_name})

include(${CMAKE_MODULE_PATH}/macros.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    add_compile_options("/std:c++latest")
    COPY_ASSIMP_DLL(${DEPENDENCIES_PATH}/lib ${PROJECT_BINARY_DIR}/bin)
elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    add_compile_options("-std=c++17")
endif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(ASSIMP REQUIRED)
#find_package(RapidJSON REQUIRED)

ADD_PROJECTS(ALL_LIBRARIES ${LIBRARIES_PATH} ${PROJECT_BINARY_DIR}/libraries)
ADD_PROJECTS(ALL_EXECUTABLES ${EXECUTABLES_PATH} ${PROJECT_BINARY_DIR}/demos)
