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

add_library(common STATIC ${COMMON_SOURCES})
add_executable(client ${CLIENT_SOURCES})
add_executable(server ${SERVER_SOURCES})

set(targets client server common)

foreach (target ${targets})
    target_compile_options(${target} PRIVATE -std=c++2a)

    set(COMMON_SOURCES_DIR common)
    aux_source_directory(${COMMON_SOURCES_DIR} COMMON_SOURCES)
    target_sources(${target} PRIVATE ${COMMON_SOURCES})

    target_include_directories(${target} PRIVATE
            ${COMMON_SOURCES_DIR} ${CONAN_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
            )

    target_precompile_headers(${target} PRIVATE stdafx.h)

    target_link_libraries(${target} PRIVATE ${CONAN_LIBS})
endforeach ()

set(exe_targets client server)

foreach(exe_target ${exe_targets})
    target_link_libraries(${exe_target} PRIVATE ${common})
endforeach()