cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME LANDR_SDK_CLI_EXAMPLE)
project(${PROJECT_NAME} VERSION 1.0.0)
set(TARGET_NAME "licenser")

set(LANDR_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../landr")

if (APPLE)
    set(LIB_SUBPATH macos)
else()
    set(LIB_SUBPATH windows)
endif()

if(WIN32)
    if (MSVC)
        add_link_options(/ignore:4099)
    endif()
endif()

# Use the presence of the target to determine whether we're doing an in-tree build
if (NOT TARGET LANDR)
    if (WIN32)
        set(LIB_LANDR "")
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            if(config STREQUAL "Debug")
                set(LIB_SUFFIX "MDd")
            else()
                set(LIB_SUFFIX "MD")
            endif()

            find_library(LIB_LANDR_${config}
                NAMES LANDR_${LIB_SUFFIX}
                HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
                PATH_SUFFIXES ${LIB_SUBPATH}
                REQUIRED
                )
            list(APPEND LIB_LANDR "$<$<CONFIG:${config}>:${LIB_LANDR_${config}}>")
        endforeach()
    else()
        find_library(LIB_LANDR
            NAMES LANDR
            HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
            PATH_SUFFIXES ${LIB_SUBPATH}
            REQUIRED
            )
    endif()
else()
    set(LIB_LANDR LANDR)
endif()

if (DEFINED WITH_CONFIG)
    set(LIB_LANDR_CONFIG LANDRConfig)
else()
    # Find the first config directory
    file(GLOB LIB_LANDR_CONFIG_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../config*/${LIB_SUBPATH})
    list(GET LIB_LANDR_CONFIG_DIRS 0 LIB_LANDR_CONFIG_DIR)

    if (WIN32)
        set(LIB_LANDR_CONFIG "${LIB_LANDR_CONFIG_DIR}/LANDRConfig_MD$<$<CONFIG:DEBUG>:d>.lib")
    else()
        set(LIB_LANDR_CONFIG "${LIB_LANDR_CONFIG_DIR}/libLANDRConfig.a")
    endif()
endif()

if (CONFIG_LIB_PATH)
    set(LIB_LANDR_CONFIG "${CONFIG_LIB_PATH}")
endif()

message(STATUS "Using final config: ${LIB_LANDR_CONFIG}")

if (APPLE)
    set(MAC_LIBS "-framework AppKit -framework Security -framework SystemConfiguration")
else()
    set(WIN_LIBS winhttp Crypt32 wininet Ws2_32)
endif()

add_executable(${TARGET_NAME} "${PROJECT_SOURCE_DIR}/main.cpp")

target_include_directories(${TARGET_NAME} PUBLIC ${LANDR_HEADERS_DIR})

target_link_libraries(${TARGET_NAME}
    PRIVATE
        ${WIN_LIBS}
        ${MAC_LIBS}
    PUBLIC
        ${LIB_LANDR}
        ${LIB_LANDR_CONFIG}
    )

