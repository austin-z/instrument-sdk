cmake_minimum_required(VERSION 3.10)
project(scissors_example)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add header files path if needed
# include_directories(include)
include_directories(src)

# Add executable target
add_executable(scissors_example ${SOURCES})

# Link pthread library
target_link_libraries(scissors_example PRIVATE pthread)

# Set output directory for the shared library
set_target_properties(scissors_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Optionally, install the library
#install(TARGETS scissors_example LIBRARY DESTINATION lib)

# Optionally, install headers
#install(DIRECTORY include/ DESTINATION include)