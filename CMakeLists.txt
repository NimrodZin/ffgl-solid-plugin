cmake_minimum_required(VERSION 3.16)
project(SolidColorFFGL VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# macOS Universal Binary support (Intel + Apple Silicon)
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for macOS")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS deployment version")
endif()

# FFGL SDK location - we'll use FetchContent to get the official SDK
include(FetchContent)

FetchContent_Declare(
    ffgl_sdk
    GIT_REPOSITORY https://github.com/resolume/ffgl.git
    GIT_TAG 2.2  # Use latest stable release tag
)
FetchContent_MakeAvailable(ffgl_sdk)

# Source files for FFGL library
set(FFGL_LIB_SOURCES
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffgl/FFGL.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffgl/FFGLPluginSDK.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffgl/FFGLThumbnailInfo.cpp  # <-- Move here (line 33)
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLAudio.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLEffect.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLMixer.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLParam.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLParamEvent.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLParamFFT.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLParamOption.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLParamRange.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLParamText.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLParamTrigger.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLPlugin.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglquickstart/FFGLSource.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLScopedFBOBinding.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLScopedSamplerActivation.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLScopedShaderBinding.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLScopedTextureBinding.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLScopedVAOBinding.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLScopedVBOBinding.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLScreenQuad.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLShader.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffglex/FFGLUtilities.cpp
)

# Plugin source files
set(PLUGIN_SOURCES
    source/plugins/SolidColor/SolidColor.cpp
    source/plugins/SolidColor/SolidColor.h
)

# Define the plugin
if(WIN32)
    # Windows: Build as DLL
    add_library(SolidColor SHARED ${PLUGIN_SOURCES} ${FFGL_LIB_SOURCES})
    set_target_properties(SolidColor PROPERTIES
        SUFFIX ".dll"
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    # Windows-specific definitions
    target_compile_definitions(SolidColor PRIVATE
        _WIN32
        WIN32
        _WINDOWS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
elseif(APPLE)
    # macOS: Build as Bundle
    add_library(SolidColor MODULE ${PLUGIN_SOURCES} ${FFGL_LIB_SOURCES})
    set_target_properties(SolidColor PROPERTIES
        BUNDLE TRUE
        BUNDLE_EXTENSION "bundle"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourname.solidcolor"
        MACOSX_BUNDLE_BUNDLE_NAME "SolidColor"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    # macOS frameworks
    find_library(OPENGL_FRAMEWORK OpenGL)
    find_library(COCOA_FRAMEWORK Cocoa)
    target_link_libraries(SolidColor PRIVATE ${OPENGL_FRAMEWORK} ${COCOA_FRAMEWORK})
endif()

# Include directories
target_include_directories(SolidColor PRIVATE
    ${ffgl_sdk_SOURCE_DIR}/source/lib
    ${ffgl_sdk_SOURCE_DIR}/deps/glew/include
)

# GLEW handling
if(WIN32)
    # On Windows, link to prebuilt GLEW
    target_link_libraries(SolidColor PRIVATE
        ${ffgl_sdk_SOURCE_DIR}/deps/glew/lib/Release/x64/glew32.lib
        opengl32
    )
    # Copy GLEW DLL to output directory
    add_custom_command(TARGET SolidColor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ffgl_sdk_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll"
        "$<TARGET_FILE_DIR:SolidColor>"
    )
elseif(APPLE)
    # On macOS, compile GLEW from source
    target_sources(SolidColor PRIVATE
        ${ffgl_sdk_SOURCE_DIR}/deps/glew/src/glew.c
    )
    target_compile_definitions(SolidColor PRIVATE GLEW_STATIC GLEW_NO_GLU)
endif()

# Compiler flags
if(MSVC)
    target_compile_options(SolidColor PRIVATE /W3 /MP)
else()
    target_compile_options(SolidColor PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation
install(TARGETS SolidColor
    RUNTIME DESTINATION bin      # Windows DLL
    LIBRARY DESTINATION lib      # macOS bundle
    BUNDLE DESTINATION plugins   # macOS bundle (alternative)
)

# CPack configuration for creating distributable packages
set(CPACK_PACKAGE_NAME "SolidColor-FFGL")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Your Name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Solid Color FFGL Plugin for Resolume Arena")
include(CPack)
