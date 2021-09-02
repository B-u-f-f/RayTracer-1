set(BINARY ${CMAKE_PROJECT_NAME}e)

file(GLOB_RECURSE SRC_FILES ./sources/*.c)

set(SOURCES ${SRC_FILES})

add_library(${LIB_NAME} STATIC ${SOURCES})
target_link_libraries(${LIB_NAME} m)
target_compile_options(${LIB_NAME} PUBLIC -Wall -Werror -Wpedantic -Wextra -g)
target_include_directories(${LIB_NAME} PUBLIC ${SOURCE_DIR}/include)

add_executable(${BINARY} main.c)
target_link_libraries(${BINARY} m ${LIB_NAME})
target_compile_options(${BINARY} PUBLIC -Wall -Werror -Wpedantic -Wextra -g)
target_include_directories(${BINARY} PUBLIC ${SOURCE_DIR}/include)
