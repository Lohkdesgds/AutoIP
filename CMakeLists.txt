cmake_minimum_required(VERSION 3.15)

project(AutoIP VERSION 1.0 DESCRIPTION "An easy to use app to keep requesting HTTP stuff in time intervals")

# Release?
set(CMAKE_BUILD_TYPE Release)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "_DEBUG")
else ()
    set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "NDEBUG")
endif ()


file(GLOB_RECURSE source_list "src/*.cpp" "include/*.hpp")
add_executable(${PROJECT_NAME} ${source_list})

target_include_directories(${PROJECT_NAME} PRIVATE 
    #httplib
    "include"
)

# Set C++ version
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
 )