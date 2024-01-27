cmake_minimum_required(VERSION 3.6.0)
project(vulkan_stuff VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32) 
    set(SDL2_DIR "E:\\SDL2\\SDL2-2.28.5\\cmake")
    set(spdlog_DIR "E:\\spdlog\\build")
endif()

find_package(SDL2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(vulkan_stuff 
main.cpp
        Engine/Window.cpp
        Engine/Window.h
        VulkanRenderer.cpp
        VulkanRenderer.h
        Engine/VulkanRenderer/Renderer.cpp
        Engine/VulkanRenderer/Renderer.h
        Engine/VulkanRenderer/Renderer.h
        Engine/VulkanRenderer/Device.cpp
        Engine/VulkanRenderer/Device.h
        Engine/VulkanRenderer/Device.h
        Engine/VulkanRenderer/Instance.cpp
        Engine/VulkanRenderer/Instance.h
        Engine/VulkanRenderer/Swapchain.cpp
        Engine/VulkanRenderer/Swapchain.h
        Engine/VulkanRenderer/Surface.cpp
        Engine/VulkanRenderer/Surface.h
)

if (WIN32) 
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS})   
else()
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS}/SDL2)
endif()

message(${Vulkan_INCLUDE_DIRS})

target_include_directories(vulkan_stuff PRIVATE ${Vulkan_INCLUDE_DIRS})
#target_include_directories(vulkan_stuff PRIVATE ${PROJECT_SOURCE_DIR}/Engine)

set(engine_dir "${PROJECT_SOURCE_DIR}/Engine/")
target_include_directories(vulkan_stuff PRIVATE "${engine_dir}")

message("${engine_dir}")

# no more diabolical compile times
target_precompile_headers(vulkan_stuff PUBLIC ${Vulkan_INCLUDE_DIRS}/vulkan/vulkan.h)
target_precompile_headers(vulkan_stuff PRIVATE ${spdlog_INCLUDE_DIRS})

# env
if (WIN32)
    set(ENV{VK_LAYER_PATH} "C:\\VulkanSDK\\1.3.275.0\\Bin")
    message("$ENV{VK_LAYER_PATH} thing yes")
endif()

target_link_libraries(vulkan_stuff PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(vulkan_stuff PRIVATE ${Vulkan_LIBRARIES})
target_link_libraries(vulkan_stuff PRIVATE spdlog::spdlog)
