cmake_minimum_required(VERSION 3.25)
project(task_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -static-libstdc++ -static-libgcc")


set(SDL2_INCLUDE_DIR C:/SDL2_64/include)
set(SDL2_LIB_DIR C:/SDL2_64/lib)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(task_1 main.cpp)

set_target_properties(task_1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/")

target_link_libraries(task_1 mingw32 SDL2main SDL2 SDL2_image zlib1 libjpeg-9 libpng16-16)