cmake_minimum_required(VERSION 3.21)

project(echo-server)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

aux_source_directory(client CLIENT_SOURCES)
aux_source_directory(client/tcp CLIENT_TCP_SOURCES)
aux_source_directory(client/udp CLIENT_UDP_SOURCES)
aux_source_directory(server SERVER_SOURCES)
aux_source_directory(server/tcp SERVER_TCP_SOURCES)
aux_source_directory(server/udp SERVER_UDP_SOURCES)

add_executable(client ${CLIENT_TCP_SOURCES} ${CLIENT_UDP_SOURCES} ${CLIENT_SOURCES})
add_executable(server ${SERVER_TCP_SOURCES} ${SERVER_UDP_SOURCES} ${SERVER_SOURCES})

set(targets client server)

foreach (target ${targets})
    target_compile_options(${target} PRIVATE -std=c++2a)

    set(COMMON_SOURCES_DIR common)
    aux_source_directory(${COMMON_SOURCES_DIR} COMMON_SOURCES)
    target_sources(${target} PRIVATE ${COMMON_SOURCES})

    target_include_directories(${target} PRIVATE
            ${COMMON_SOURCES_DIR} ${CONAN_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
            )

    target_precompile_headers(${target} PRIVATE stdafx.h)

    target_link_libraries(${target} PRIVATE boost_system ${CONAN_SYSTEM_LIBS})
endforeach ()
