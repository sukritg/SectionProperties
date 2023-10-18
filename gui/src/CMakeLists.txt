 # Basic Project Settings
 cmake_minimum_required(VERSION 3.26)
 project(SectionProperties CXX)
 set(CMAKE_CXX_STANDARD 20)
 set(CMAKE_CXX_STANDARD_REQUIRED  ON)

 # wxWidgets library
 find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
 include(${wxWidgets_USE_FILE})

 if(WIN32)
     add_executable(APP WIN32)
 else ()
     add_executable(APP MACOSX_BUNDLE)
 endif ()
target_sources(APP PUBLIC app.cpp mainwnd.cpp graphics2d.cpp input.cpp output.cpp  imagepanel.cpp)
target_link_libraries(APP PUBLIC ${wxWidgets_LIBRARIES} xprop)
target_include_directories(APP PUBLIC ${INCLUDES})
