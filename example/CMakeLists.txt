# Remove all instructions
project(Library-Template CXX)               #  Rename Project name

add_executable(App main.cpp)                # Update program cpp filename
target_link_libraries(App PRIVATE test)     # Rename Library and Program name
set_target_properties(App PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")    # Rename executable name

