cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/CSFDemo ${PCL_INCLUDE_DIRS})

# Set target C++ standard and required compiler features
set(CMAKE_CXX_STANDARD 14 CACHE STRING "The target C++ standard. PCL requires C++14 or higher.")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PCL_CXX_COMPILE_FEATURES cxx_std_14)
include_directories(SYSTEM)

add_compile_options("-O3")
find_package(PCL 1.10 REQUIRED)
# include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

set(APP_SRC CSFDemo.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(csfdemo ${APP_SRC})
target_link_libraries(csfdemo CSF  ${PCL_LIBRARIES})