cmake_minimum_required(VERSION 3.0.2)
project(GraviStile)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(GRAVISTILE_SRC_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB gs_src ${GRAVISTILE_SRC_DIR}/*.*)

#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS graphics window system REQUIRED)
message(STATUS "SFML found: ${SFML_FOUND}")

add_executable(GraviStile ${gs_src})
target_link_libraries(GraviStile PUBLIC ${SFML_LIBRARIES})
