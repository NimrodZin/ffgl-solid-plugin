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

# FFGL SDK location - use local copy
set(ffgl_sdk_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/ffgl)

# Source files for FFGL library
set(FFGL_LIB_SOURCES
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffgl/FFGL.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffgl/FFGLPluginSDK.cpp
    ${ffgl_sdk_SOURCE_DIR}/source/lib/ffgl/FFGLThumbnailInfo.cpp
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

# Rest of your CMakeLists.txt stays the same...
