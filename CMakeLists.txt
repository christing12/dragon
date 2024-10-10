cmake_minimum_required(VERSION 3.20)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # sets .lib files to be output here
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # sets .exe files to be output here

set(PROJECT_NAME dragon)
project(${PROJECT_NAME} 
    VERSION 0.0.0
    DESCRIPTION "Vulkan Rendering Engine"
    LANGUAGES CXX
)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

