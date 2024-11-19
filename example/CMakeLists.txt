set(PROJECT_NAME LANDR_SDK_EXAMPLE)
set(PLUGIN_NAME "LANDR SDK Example")
project(${PROJECT_NAME} VERSION 1.0.0)

# Fetch JUCE
FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.3
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(${PROJECT_NAME}
    COPY_PLUGIN_AFTER_BUILD TRUE                # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE LNDR               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Dem0                            # A unique four-character plugin id with exactly one upper-case character
    FORMATS AU VST3 AAX Standalone              # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME ${PLUGIN_NAME}                 # The name of the final executable, which can differ from the target name
    PLUGIN_NAME ${PLUGIN_NAME}
    COMPANY_NAME "LANDR"
    VST3_AUTO_MANIFEST FALSE)

target_sources(${PROJECT_NAME}
    PRIVATE
        "${PROJECT_SOURCE_DIR}/src/PluginEditor.h"
        "${PROJECT_SOURCE_DIR}/src/PluginEditor.cpp"
        "${PROJECT_SOURCE_DIR}/src/PluginProcessor.h"
        "${PROJECT_SOURCE_DIR}/src/PluginProcessor.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC ${LANDR_SDK_DIR})

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        LANDRConfig
        LANDR)
