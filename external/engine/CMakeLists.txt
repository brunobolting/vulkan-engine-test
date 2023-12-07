cmake_minimum_required(VERSION 3.5)
project(cpp_engine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# add external libraries
add_subdirectory(external/glm)
add_subdirectory(external/glfw)
add_subdirectory(external/vk-bootstrap)

# library find functions
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        glm
    PRIVATE
        ${Vulkan_LIBRARIES}
        glfw
        vk-bootstrap
)
