project(stromx-studio)

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set (STROMX_STUDIO_VERSION_MAJOR 0)
set (STROMX_STUDIO_VERSION_MINOR 1)
set (STROMX_STUDIO_VERSION_PATCH 0)

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror")
endif(CMAKE_COMPILER_IS_GNUCXX)

include(UseDoxygen OPTIONAL)

find_package(Qt4 REQUIRED)
find_package(stromx REQUIRED)

if(UNIX)
    add_definitions(-DUNIX)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_subdirectory(src)
