project(stromx-gui)

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(Qt4 REQUIRED)
find_package(stromx REQUIRED)

add_subdirectory(src)
