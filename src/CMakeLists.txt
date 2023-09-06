# Remove all instructions
project(Library-Template CXX)                                   #  Rename Project name

add_library(test)                                               # Rename library name
target_include_directories(test PUBLIC ${INCLUDE_PATH})         # Rename library name
set_target_properties(test PROPERTIES ARCHIVE_OUTPUT_DIRECTORY  # Rename library name
        "${CMAKE_BINARY_DIR}/lib")
target_sources(test PUBLIC test.cpp)                            # Rename library name
