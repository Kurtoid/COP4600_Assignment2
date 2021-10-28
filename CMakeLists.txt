cmake_minimum_required (VERSION 2.8.12)
project (assignment2)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set (CMAKE_CXX_STANDARD 17)

include_directories(includes)

add_library (commands src/commands.cpp)
add_library (history src/history.cpp)
add_library (foldertools src/foldertools.cpp)
add_library (programtools src/programtools.cpp)

add_executable(assignment2 src/assignment2.cpp)
target_link_libraries(assignment2 commands)
target_link_libraries(assignment2 history)
target_link_libraries(assignment2 foldertools)
target_link_libraries(assignment2 programtools)