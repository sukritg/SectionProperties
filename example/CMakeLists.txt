 # Basic Project Settings
 cmake_minimum_required(VERSION 3.26)
 project(SectionProperties CXX)
 set(CMAKE_CXX_STANDARD 20)
 set(CMAKE_CXX_STANDARD_REQUIRED  ON)


 add_executable(APP_CONSOLE main.cpp)
target_link_libraries(APP_CONSOLE PRIVATE xprop)


