cmake_minimum_required(VERSION 3.10)
project(instruments)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add header files path if needed
include_directories(include)
include_directories(src)

# Add executable target
add_library(instruments SHARED ${SOURCES})

# Link pthread library
target_link_libraries(instruments PRIVATE pthread)

# Set output directory for the shared library
set_target_properties(instruments PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/sdk)

set_target_properties(instruments PROPERTIES VERSION 0.0.1)

add_custom_target(copy_directory ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/sdk/include
)

# Optionally, install the library
#install(TARGETS instruments LIBRARY DESTINATION lib)

# Optionally, install headers
#install(DIRECTORY include/ DESTINATION include)