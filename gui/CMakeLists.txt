 # Basic Project Settings
 cmake_minimum_required(VERSION 3.26)
 project(SectionProperties CXX)
 set(CMAKE_CXX_STANDARD 20)
 set(CMAKE_CXX_STANDARD_REQUIRED  ON)

 # header files
 include_directories(include)
 add_subdirectory(src)

 file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/gui)