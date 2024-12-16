if (WITH_CLI_EXAMPLE)
add_subdirectory(commandline)
endif()

if (WITH_JUCE_EXAMPLE)
add_subdirectory(plugin)
endif()