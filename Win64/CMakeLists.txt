cmake_minimum_required(VERSION 3.1...3.15)
project(Rigonomicon)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

cmake_policy(SET CMP0048 NEW)


set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cgcmake/modules)

add_subdirectory(src)