project(stromx-studio)

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(UseDoxygen OPTIONAL)

find_package(Qt4 REQUIRED)
find_package(stromx REQUIRED)

find_package(CppUnit)

if(CPPUNIT_FOUND)
    option(BUILD_TESTS "Build unit tests" ON)
endif()

add_subdirectory(src)
