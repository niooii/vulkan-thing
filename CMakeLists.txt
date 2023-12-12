cmake_minimum_required(VERSION 3.0.0)
project(vulkan_stuff VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)

find_package(SDL2 REQUIRED)
find_package(Vulkan REQUIRED)

message(VULKAN_STUFF="${Vulkan_LIBRARIES}")

include_directories(engine ${SDL2_INCLUDE_DIRS})   

add_executable(vulkan_stuff 
main.cpp
Window.cpp
Window.h
VulkanRenderer.cpp
VulkanRenderer.h)

target_link_libraries(vulkan_stuff ${SDL2_LIBRARIES})
target_link_libraries(vulkan_stuff ${Vulkan_LIBRARIES})

message(SDL2_LIBRARIES="${SDL2_LIBRARIES}")

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
#