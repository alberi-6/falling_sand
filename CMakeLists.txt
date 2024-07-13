cmake_minimum_required(VERSION 3.10.0)
project(falling_sand VERSION 0.1.0)

set(SOURCES main.cpp src/RenderWindow.cpp)
set(HEADERS include/RenderWindow.hpp)

# for telling cmake where to locate FindSDL2_image.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_executable(falling_sand ${SOURCES} ${HEADERS})

target_include_directories(falling_sand PRIVATE include)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(falling_sand PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
