cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pdc VERSION 0.0)
file(GLOB_RECURSE SRC_FILES ./src/*.c)

add_executable(pdc ${SRC_FILES})
target_link_libraries(pdc m)
target_compile_options(pdc PUBLIC -Wall -Werror -Wpedantic -Wextra)
target_include_directories(pdc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/include)
