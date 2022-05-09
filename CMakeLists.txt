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

foreach(SOURCE_DIR ${CLIENT_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} CLIENT_SOURCES)
endforeach()

foreach(SOURCE_DIR ${SERVER_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} SERVER_SOURCES)
endforeach()

foreach(SOURCE_DIR ${COMMON_SOURCE_DIRS})
    aux_source_directory(${SOURCE_DIR} COMMON_SOURCES)
endforeach()

add_executable(client ${CLIENT_SOURCES})
add_executable(server ${SERVER_SOURCES})

add_library(common STATIC ${COMMON_SOURCES})
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

set(exe_targets client server)
set(targets ${exe_targets} common)

foreach (target ${targets})
    target_compile_options(${target} PRIVATE -std=c++2a)

    target_include_directories(
        ${target} PRIVATE
        ${CONAN_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach ()

foreach(target ${exe_targets})
    target_link_libraries(${target} PRIVATE boost_asio common ${CONAN_LIBS})
endforeach()