cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE PATH "Project specific path. Set manually if it was not found.")
set(EXECUTABLES_PATH ${CMAKE_SOURCE_DIR}/demos CACHE PATH "Project specific path. Set manually if it was not found.")
set(LIBRARIES_PATH ${CMAKE_SOURCE_DIR}/src/libraries CACHE PATH "Project specific path. Set manually if it was not found.")
set(DEPENDENCIES_PATH ${CMAKE_SOURCE_DIR}/dependencies CACHE PATH "Project specific path. Set manually if it was not found.")
set(RESOURCE_PATH ../../resources CACHE PATH "Project specific path. Set manually if it was not found.")
set(SHADER_PATH ../../src/shader CACHE PATH "Project specific path. Set manually if it was not found.")
#set(TESTS_PATH ${CMAKE_SOURCE_DIR}/tests CACHE PATH "Project specific path. Set manually if it was not found.")

include(${CMAKE_MODULE_PATH}/complete_project.cmake)
