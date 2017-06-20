cmake_minimum_required(VERSION 3.7)
project(CMakeGenerator)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)
add_executable(CMakeGenerator ${SOURCE_FILES})
target_link_libraries(CMakeGenerator
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        )
