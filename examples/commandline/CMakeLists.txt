cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME LANDR_SDK_CLI_EXAMPLE)
project(${PROJECT_NAME} VERSION 1.0.0)
set(TARGET_NAME "landrsdk-example")

set(LANDR_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../landr")

if (APPLE)
    set(LIB_SUBPATH macos)
else()
    set(LIB_SUBPATH windows)
endif()

# Use the presence of the target to determine whether we're doing an in-tree build
if (NOT TARGET LANDR)
    find_library(LIB_LANDR
        NAMES LANDR
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../lib
        PATH_SUFFIXES ${LIB_SUBPATH}
        REQUIRED
    )
else()
    set(LIB_LANDR LANDR)
endif()

if (DEFINED WITH_CONFIG)
    set(LIB_LANDR_CONFIG LANDRConfig)
else()
    find_library(LIB_LANDR_CONFIG
        NAMES LANDRConfig
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../config_example
        PATH_SUFFIXES ${LIB_SUBPATH}
        REQUIRED
    )
endif()

if (APPLE)
    find_library(LIB_SYSTEM_CONFIGURATION SystemConfiguration)
    if (NOT LIB_SYSTEM_CONFIGURATION)
        message(FATAL_ERROR "SystemConfiguration framework not found")
    endif()
    set(MAC_LIBS "-framework AppKit")
else()
    set(WIN_LIBS winhttp Crypt32 wininet.lib)
endif()

add_executable(${TARGET_NAME} "${PROJECT_SOURCE_DIR}/main.cpp")

target_include_directories(${TARGET_NAME} PUBLIC ${LANDR_HEADERS_DIR})

target_link_libraries(${TARGET_NAME}
    PRIVATE
        ${LIB_SYSTEM_CONFIGURATION}
        ${WIN_LIBS}
        ${MAC_LIBS}
    PUBLIC
        ${LIB_LANDR}
        ${LIB_LANDR_CONFIG}
    )
    
