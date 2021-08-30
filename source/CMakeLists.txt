set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES ./src/*.c)

set(SOURCES ${SRC_FILES})

add_library(${BINARY}_lib STATIC ${SOURCES})
target_link_libraries(${BINARY}_lib m)
target_compile_options(${BINARY}_lib PUBLIC -Wall -Werror -Wpedantic -Wextra -g)
target_include_directories(${BINARY}_lib PUBLIC ${SOURCE_DIR}/include)

add_executable(${BINARY}_run main.c)
target_link_libraries(${BINARY}_run m ${BINARY}_lib)
target_compile_options(${BINARY}_run PUBLIC -Wall -Werror -Wpedantic -Wextra -g)
target_include_directories(${BINARY}_run PUBLIC ${SOURCE_DIR}/include)
