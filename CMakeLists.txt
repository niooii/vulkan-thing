cmake_minimum_required(VERSION 3.6.0)
project(vulkan_stuff VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32) 
    set(SDL2_DIR "E:\\clibs\\SDL2-2.28.5\\cmake")
endif()

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(vulkan_stuff 
main.cpp
Window.cpp
Window.h
VulkanRenderer.cpp
VulkanRenderer.h)

if (WIN32) 
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS})   
else()
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS}/SDL2)
endif()

target_include_directories(vulkan_stuff PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_libraries(vulkan_stuff ${SDL2_LIBRARIES})
target_link_libraries(vulkan_stuff ${Vulkan_LIBRARIES})
