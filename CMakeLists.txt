cmake_minimum_required(VERSION 3.10)
project(Socker C)

# Set the output directory for build files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the static library
file(GLOB SRC_FILES "src/*.c")
add_library(socker STATIC ${SRC_FILES})
target_include_directories(socker PUBLIC include)

# Function to add test executables
function(add_test_executables test_dir)
    file(GLOB TEST_SOURCES "${test_dir}/*.c")
    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        add_executable(${test_name} ${test_file})
        target_link_libraries(${test_name} socker)
        target_include_directories(${test_name} PRIVATE include)
    endforeach()
endfunction()

# Add test executables from various directories
add_test_executables(test)
add_test_executables(test/http_client)
add_test_executables(test/mode_concurrent/mono_service)
add_test_executables(test/mode_connecte_tcp)
add_test_executables(test/mode_non_connecte_udp)
