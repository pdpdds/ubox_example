# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(../jni/sdl2/include)
include_directories(../jni/sdl2_ttf)
include_directories(../jni/sdl2_image)
include_directories(../jni/sdl2_mixer)
include_directories(../jni/iconv/include)
include_directories(../jni/iconv/include)

include_directories(../../../include)
include_directories(../../src)
include_directories(../../generated)
include_directories(../../../tools/apultra/src/libdivsufsort/include)
include_directories(../../../tools/apultra/src)

# configure import libs
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../libs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
add_library( main
             SHARED
             ../../msvc/src/ubox_sdl.c
             ../../msvc/src/ubox_mplayer_sdl.c
             ../../msvc/src/ubox_spman_sdl.cpp
             ../../src/main.c
             ../../src/helpers.c
             ../../src/data.c
             ../../../tools/apultra/src/libdivsufsort/lib/divsufsort.c
             ../../../tools/apultra/src/libdivsufsort/lib/divsufsort_utils.c
             ../../../tools/apultra/src/libdivsufsort/lib/sssort.c
             ../../../tools/apultra/src/libdivsufsort/lib/trsort.c
             ../../../tools/apultra/src/expand.c
             ../../../tools/apultra/src/matchfinder.c
             ../../../tools/apultra/src/shrink.c
             ../../src/game.c)
set_target_properties(main PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/${ANDROID_ABI}/main.so)

add_library(lib_iconv SHARED IMPORTED)
set_target_properties(lib_iconv PROPERTIES IMPORTED_LOCATION
${distribution_DIR}/${ANDROID_ABI}/libiconv.so)

add_library(lib_sdl2 SHARED IMPORTED)
set_target_properties(lib_sdl2 PROPERTIES IMPORTED_LOCATION
${distribution_DIR}/${ANDROID_ABI}/libSDL2.so)

add_library(lib_sdl2image SHARED IMPORTED)
set_target_properties(lib_sdl2image PROPERTIES IMPORTED_LOCATION
${distribution_DIR}/${ANDROID_ABI}/libSDL2_image.so)

add_library(lib_sdl2mixer SHARED IMPORTED)
set_target_properties(lib_sdl2mixer PROPERTIES IMPORTED_LOCATION
${distribution_DIR}/${ANDROID_ABI}/libSDL2_mixer.so)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       main
                       lib_iconv
                       lib_sdl2
                       lib_sdl2image
                       lib_sdl2mixer
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )

