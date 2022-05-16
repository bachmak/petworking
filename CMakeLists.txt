cmake_minimum_required(VERSION 3.21)

project(echo-server)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set (
    CLIENT_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/client
    ${CMAKE_CURRENT_SOURCE_DIR}/client/tcp
    ${CMAKE_CURRENT_SOURCE_DIR}/client/udp
)

set (
    SERVER_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/server
    ${CMAKE_CURRENT_SOURCE_DIR}/server/tcp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/udp
)

set (
    COMMON_SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

set (
    TEST_HELPERS_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/test/helpers/scenarios
)

foreach(SOURCE_DIR ${CLIENT_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} CLIENT_SOURCES)
endforeach()

foreach(SOURCE_DIR ${SERVER_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} SERVER_SOURCES)
endforeach()

foreach(SOURCE_DIR ${COMMON_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} COMMON_SOURCES)
endforeach()

foreach(SOURCE_DIR ${TEST_HELPERS_DIRS})
    aux_source_directory(${SOURCE_DIR} TEST_HELPERS_SOURCES) 
endforeach()

enable_testing()
include(GoogleTest)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/test TEST_SOURCES)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE}) 
    list(APPEND tests ${TEST_NAME})
    gtest_discover_tests(${TEST_NAME})
endforeach()

add_library(client STATIC ${CLIENT_SOURCES})
add_library(server STATIC ${SERVER_SOURCES})
add_library(common STATIC ${COMMON_SOURCES})
add_library(test_helpers STATIC ${TEST_HELPERS_SOURCES})
add_library(boost_asio STATIC boost_asio.cpp)

add_compile_definitions(BOOST_ASIO_SEPARATE_COMPILATION)

file(GLOB_RECURSE
    ALL_SOURCES
    *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
)

add_custom_command(
    TARGET common
    COMMAND /usr/bin/clang-format -i -style=file ${ALL_SOURCES}
)

target_precompile_headers(common PRIVATE stdafx.h)
target_precompile_headers(server REUSE_FROM common)
target_precompile_headers(client REUSE_FROM common)
target_precompile_headers(test_helpers REUSE_FROM common)

set(libs test_helpers client server common boost_asio)
set(targets ${libs} ${tests})

foreach (target ${targets})
    target_compile_options(${target} PRIVATE -std=c++20)

    target_include_directories(
        ${target} PRIVATE
        ${CONAN_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach ()

foreach(test ${tests})
    target_link_libraries(${test} PRIVATE ${libs} ${CONAN_LIBS})
    message(STATUS ${test})
endforeach()

foreach(lib ${CONAN_LIBS})
    message(STATUS ${lib})
endforeach()