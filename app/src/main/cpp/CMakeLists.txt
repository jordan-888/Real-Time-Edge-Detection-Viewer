cmake_minimum_required(VERSION 3.22.1)

project("edgedetector")

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV configuration
# Note: Set OPENCV_SDK_PATH via command line or environment variable
# Example: -DOPENCV_SDK_PATH=/path/to/opencv-android-sdk
set(OPENCV_SDK_PATH "$ENV{OPENCV_SDK_PATH}" CACHE PATH "Path to OpenCV Android SDK")

if(NOT EXISTS ${OPENCV_SDK_PATH})
    message(WARNING "OpenCV SDK not found at ${OPENCV_SDK_PATH}. Please set OPENCV_SDK_PATH.")
    message(WARNING "Download from: https://github.com/opencv/opencv/releases")
endif()

# Include OpenCV headers
include_directories(${OPENCV_SDK_PATH}/sdk/native/jni/include)

# Add OpenCV library
add_library(opencv_java4 SHARED IMPORTED)
set_target_properties(opencv_java4 PROPERTIES
    IMPORTED_LOCATION ${OPENCV_SDK_PATH}/sdk/native/libs/${ANDROID_ABI}/libopencv_java4.so)

# Find required Android libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv2-lib GLESv2)
find_library(EGL-lib EGL)

# Add source files
file(GLOB_RECURSE NATIVE_SOURCES
    "jni/*.cpp"
    "gl/*.cpp"
)

# Create native library
add_library(${CMAKE_PROJECT_NAME} SHARED
    ${NATIVE_SOURCES}
    native-lib.cpp
)

# Link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    opencv_java4
    ${log-lib}
    ${android-lib}
    ${GLESv2-lib}
    ${EGL-lib}
)
