# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project(maestro)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(APP_SOURCES
        maestro-bridge.cpp
        AudioPlayer.cpp
        Maestro.cpp
        audio/DataSource.cpp
        audio/AAssetDataSource.cpp)

add_library( # Sets the name of the library.
        maestro

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${APP_SOURCES})

set(FFMPEG_DIR /Users/ademir/workspace/fuze/ffmpeg/build/${ANDROID_ABI})
include_directories(maestro ${FFMPEG_DIR}/include)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libavformat.so)
add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libavutil.so)
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libavcodec.so)
add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libswresample.so)

target_sources(maestro PRIVATE audio/FFMpegExtractor.cpp)
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_package(oboe REQUIRED CONFIG)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
find_library(android-lib android)
target_link_libraries( # Specifies the target library.
        maestro

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${android-lib}
        avformat
        avutil
        avcodec
        swresample
        oboe::oboe)