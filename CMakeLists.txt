cmake_minimum_required (VERSION 2.8.11)
project (assignment2)

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_library (commands src/commands.cpp)
include_directories(includes)

add_executable(assignment2 src/assignment2.cpp)
target_link_libraries(assignment2 commands)