cmake_minimum_required(VERSION 3.6.0)
project(vulkan_stuff VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32) 
    set(SDL2_DIR "E:\\SDL2\\SDL2-2.28.5\\cmake")
endif()

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(vulkan_stuff 
main.cpp
        Engine/Window.cpp
        Engine/Window.h
        VulkanRenderer.cpp
        VulkanRenderer.h
        Logger.h
        Logger.cpp
        Globals.h
        Globals.cpp
        Engine/VulkanRenderer/Renderer.cpp
        Engine/VulkanRenderer/Renderer.h
        Engine/VulkanRenderer/Renderer.h
        Engine/VulkanRenderer/Device.cpp
        Engine/VulkanRenderer/Device.h
        Engine/VulkanRenderer/Device.h
        Engine/VulkanRenderer/Instance.cpp
        Engine/VulkanRenderer/Instance.h)

if (WIN32) 
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS})   
else()
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS}/SDL2)
endif()

target_include_directories(vulkan_stuff PRIVATE ${Vulkan_INCLUDE_DIRS})
#target_include_directories(vulkan_stuff PRIVATE ${PROJECT_SOURCE_DIR}/Engine)

target_precompile_headers(vulkan_stuff PUBLIC ${Vulkan_INCLUDE_DIRS}/vulkan/vulkan.hpp)

target_link_libraries(vulkan_stuff ${SDL2_LIBRARIES})
target_link_libraries(vulkan_stuff ${Vulkan_LIBRARIES})
