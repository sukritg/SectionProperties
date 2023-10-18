 # Basic Project Settings
 cmake_minimum_required(VERSION 3.26)
 project(SectionProperties CXX)
 set(CMAKE_CXX_STANDARD 20)
 set(CMAKE_CXX_STANDARD_REQUIRED  ON)

 # wxWidgets library
 find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
 include(${wxWidgets_USE_FILE})


 set(wxWidgets_FOUND  ${wxWidgets_FOUND} PARENT_SCOPE)
 set(wxWidgets_INCLUDE_DIRS  ${wxWidgets_INCLUDE_DIRS} PARENT_SCOPE)
 set(wxWidgets_USE_FILE ${wxWidgets_USE_FILE} PARENT_SCOPE)
