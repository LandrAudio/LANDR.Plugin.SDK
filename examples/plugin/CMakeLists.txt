cmake_minimum_required(VERSION 3.26)

set(PROJECT_NAME LANDR_SDK_PLUGIN_EXAMPLE)
set(PLUGIN_NAME "LANDR SDK Plugin Example")
project(${PROJECT_NAME} VERSION 1.0.0)

Include(FetchContent)

# Fetch JUCE
FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.3
)
FetchContent_MakeAvailable(JUCE)

if(WIN32)
    if (MSVC)
        add_link_options(/ignore:4099)
    endif()
endif()

juce_add_plugin(${PROJECT_NAME}
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE LNDR
    PLUGIN_CODE Dem0
    FORMATS AU VST3 AAX Standalone
    PRODUCT_NAME ${PLUGIN_NAME}
    PLUGIN_NAME ${PLUGIN_NAME}
    COMPANY_NAME "LANDR"
    VST3_AUTO_MANIFEST FALSE)

target_sources(${PROJECT_NAME}
    PRIVATE
        "${PROJECT_SOURCE_DIR}/src/PluginEditor.h"
        "${PROJECT_SOURCE_DIR}/src/PluginEditor.cpp"
        "${PROJECT_SOURCE_DIR}/src/PluginProcessor.h"
        "${PROJECT_SOURCE_DIR}/src/PluginProcessor.cpp")

set(LANDR_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../landr")

target_include_directories(${PROJECT_NAME} PUBLIC ${LANDR_HEADERS_DIR})

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

if (APPLE)
    set(LIB_SUBPATH macos)
else()
    set(LIB_SUBPATH windows)
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

if (APPLE)
    set(MAC_LIBS "-framework SystemConfiguration")
else()
    set(WIN_LIBS winhttp Crypt32)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        ${WIN_LIBS}
        ${MAC_LIBS}
    PUBLIC
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        ${LIB_LANDR}
        ${LIB_LANDR_CONFIG}
    )
