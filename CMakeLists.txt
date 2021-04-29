cmake_minimum_required(VERSION 3.10)
project(connect_four) # project name

set(CMAKE_CXX_STANDARD 17) ## C++ version

# creates a variable called SOURCES_CXX
# the variable is a list of files
# the list of source files we're going to compile
set(SOURCES_CXX
    src/main.cpp
)

# another variable include directories
set(INCLUDE_DIRS
    src/
)

# creates a new target called connect_four
# with all the sources specified in the SOURCES_CXX variable
add_executable(connect_four ${SOURCES_CXX})

# attach the include directories to our target
target_include_directories(connect_four PRIVATE ${INCLUDE_DIRS})