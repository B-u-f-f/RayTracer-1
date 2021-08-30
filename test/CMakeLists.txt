set(BINARY ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SRC_FILES ./*.c)

set(SOURCES ${TEST_SRC_FILES})

add_executable(${BINARY} util_test.c)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} m ${CMAKE_PROJECT_NAME}_lib CONAN_PKG::libcheck)

target_compile_options(${BINARY} PUBLIC -Wall -Werror -Wpedantic -Wextra -g)
target_include_directories(${BINARY} PUBLIC ${SOURCE_DIR}/include)
