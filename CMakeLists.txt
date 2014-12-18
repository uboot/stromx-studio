project(stromx-studio)

cmake_minimum_required(VERSION 2.8)
enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set (STROMX_STUDIO_VERSION_MAJOR 0)
set (STROMX_STUDIO_VERSION_MINOR 5)
set (STROMX_STUDIO_VERSION_PATCH 0)

if(NOT DOC_INSTALL_DIR)
    set(DOC_INSTALL_DIR "" CACHE FILEPATH "Documentation install directory")
endif()

if(MSVC)
    add_definitions(/DBOOST_ALL_DYN_LINK)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275")
endif(MSVC)

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra")
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror")
endif(CMAKE_COMPILER_IS_GNUCXX)

include(UseDoxygen OPTIONAL)

find_package(Qt4)
find_package(Qt5Widgets)
find_package(Qt5Concurrent)
find_package(Qt5Test)

if(Qt5Widgets_FOUND AND Qt5Concurrent_FOUND)
    set(USE_QT5 true)
    set(CMAKE_AUTOMOC ON)

    if(Qt5Test_FOUND)
        option(BUILD_TESTS "Build unit tests" ON)
    endif()
elseif(QT4_FOUND)
    set(USE_QT5 false)
    add_definitions(-DSTROMX_STUDIO_QT4)
    option(BUILD_TESTS "Build unit tests" ON)
else()
    message(FATAL_ERROR "Qt was not found.")
endif()

find_package(stromx REQUIRED)

if(UNIX)
    add_definitions(-DUNIX)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(desktop)

if(WIN32)
    install (FILES ${PROJECT_SOURCE_DIR}/wix/stromx-studio.wxs DESTINATION .)
endif(WIN32)