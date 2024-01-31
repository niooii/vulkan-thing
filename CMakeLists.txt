cmake_minimum_required(VERSION 3.14)
project(vulkan_stuff VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32) 
    set(SDL2_DIR "E:\\SDL2\\SDL2-2.28.5\\cmake")
#    set(spdlog_DIR "E:\\spdlog\\build")
#    set(spdlog_win_INCLUDE_DIRS "E:\\spdlog\\include\\spdlog")
endif()

find_package(SDL2 REQUIRED)
add_subdirectory(External/spdlog)
find_package(spdlog)
find_package(Vulkan REQUIRED)

# compile shaders
# shit doesnt even work rn just use bash script smh

#if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
#    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
#else()
#    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
#endif()

#if (WIN32)
#    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
#else ()
#    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/bin/glslangValidator")
#endif()
#
#message("ENV: $ENV{VULKAN_SDK}")
#
#message("GLSL VALIDATOR: ${GLSL_VALIDATOR}")
#
#file(GLOB_RECURSE GLSL_SOURCE_FILES
#        "shaders/*.frag"
#        "shaders/*.vert"
#)
#
#message("SOURCEFILES: ${GLSL_SOURCE_FILES}")
#
#foreach(GLSL ${GLSL_SOURCE_FILES})
#    get_filename_component(FILE_NAME ${GLSL} NAME)
#    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
#    add_custom_command(
#            OUTPUT ${SPIRV}
#            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
#            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
#            DEPENDS ${GLSL})
#    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
#endforeach(GLSL)
#
#add_custom_target(
#        Shaders
#        DEPENDS ${SPIRV_BINARY_FILES}
#)
#
#message("BINARYFILES: ${SPIRV_BINARY_FILES}")

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
        Engine/VulkanRenderer/Pipeline.cpp
        Engine/VulkanRenderer/Pipeline.h
        Engine/Utils.cpp
        Engine/Utils.h
)

# add_dependencies(vulkan_stuff Shaders)

if (WIN32)
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS})
    target_include_directories(vulkan_stuff PRIVATE ${spdlog_win_INCLUDE_DIRS})
else()
    target_include_directories(vulkan_stuff PRIVATE ${SDL2_INCLUDE_DIRS}/SDL2)
endif()

message(${Vulkan_INCLUDE_DIRS})

target_include_directories(vulkan_stuff PRIVATE ${Vulkan_INCLUDE_DIRS})
#target_include_directories(vulkan_stuff PRIVATE ${PROJECT_SOURCE_DIR}/Engine)

set(engine_dir "${PROJECT_SOURCE_DIR}/Engine/")
target_include_directories(vulkan_stuff PRIVATE "${engine_dir}")
# include the fmt stuff for spdlog holy shit
target_include_directories(vulkan_stuff PRIVATE "${engine_dir}")

message("${engine_dir}")

# no more diabolical compile times
target_precompile_headers(vulkan_stuff PUBLIC ${Vulkan_INCLUDE_DIRS}/vulkan/vulkan.h)
target_precompile_headers(vulkan_stuff PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/External/spdlog/include/spdlog/spdlog.h>)

# env
if (WIN32)
    set(ENV{VK_LAYER_PATH} "C:\\VulkanSDK\\1.3.275.0\\Bin")
    message("$ENV{VK_LAYER_PATH} thing yes")
endif()

target_link_libraries(vulkan_stuff spdlog)
target_link_libraries(vulkan_stuff ${SDL2_LIBRARIES})
target_link_libraries(vulkan_stuff ${Vulkan_LIBRARIES})

