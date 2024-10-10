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
find_package(Vulkan REQUIRED)
add_subdirectory(external)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE volk::volk vk-bootstrap::vk-bootstrap glfw3 glfw3_mt)
target_include_directories(${PROJECT_NAME} PRIVATE external/volk external/vkbootstrap/src external/glfw/include)
