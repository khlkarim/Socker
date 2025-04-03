cmake_minimum_required(VERSION 3.10)
project(MyStaticLibrary C)

# Set the output directory for build files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the static library
add_library(mylib STATIC 
    src/log.c
    src/utils.c
    src/endpoint.c
    src/client.c
    src/server.c
)
target_include_directories(mylib PUBLIC include)

# Collect all test files
file(GLOB TEST_SOURCES "test/mode_non_connecte_udp/*.c")
foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} mylib)
    target_include_directories(${test_name} PRIVATE include)
endforeach()