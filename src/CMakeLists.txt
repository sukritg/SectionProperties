 # Basic Project Settings
 cmake_minimum_required(VERSION 3.26)
 project(SectionProperties CXX)
 set(CMAKE_CXX_STANDARD 20)
 set(CMAKE_CXX_STANDARD_REQUIRED  ON)

 add_library(xprop STATIC section.cpp matrix.cpp point.cpp)
 target_include_directories(xprop PUBLIC ${INCLUDE_PATH})
 #set_target_properties(xprop PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
