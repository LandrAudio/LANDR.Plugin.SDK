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
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE INTERNAL "")
    if (MSVC)
        add_link_options(/ignore:4099)
    endif()
endif()

# Use the presence of the target to determine whether we're doing an in-tree build
if (NOT TARGET LANDR)
    find_library(LIB_LANDR
        NAMES LANDR
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
        PATH_SUFFIXES ${LIB_SUBPATH}
        REQUIRED
    )
else()
    set(LIB_LANDR LANDR)
endif()

if (DEFINED WITH_CONFIG)
    set(LIB_LANDR_CONFIG LANDRConfig)
else()
    # Get the first config file we find
    file(GLOB LIB_LANDR_CONFIGS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/../../../config*/${LIB_SUBPATH}/*)
    list(GET LIB_LANDR_CONFIGS 0 LIB_LANDR_CONFIG)
endif()

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

