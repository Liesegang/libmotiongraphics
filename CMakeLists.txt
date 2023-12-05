cmake_minimum_required(VERSION 3.0)
project(libmotiongraphics CXX)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
endif()

set(BOOST_ROOT "C:/Dev/boost-bin")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include/boost-1_83/")
include_directories(${BOOST_INCLUDEDIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(C:/Dev/skia)
link_directories(C:/Dev/skia/out/release)
link_libraries(
    compression_utils_portable
    expat
    harfbuzz
    icu
    libjpeg
    libpng
    libwebp
    libwebp_sse41
    particles
    pathkit
    skia
    skottie
    skparagraph
    skresources
    sksg
    skshaper
    sktext
    skunicode
    svg
    zlib
)

set(FFMPEG_DEV_ROOT "C:/Dev/ffmpeg")
include_directories(${FFMPEG_DEV_ROOT}/include)
link_directories(${FFMPEG_DEV_ROOT}/lib)
link_libraries(
    avcodec
    avformat
    avfilter
    avdevice
    swresample
    swscale
    avutil
)

link_libraries(
    opengl32
)

file(GLOB ffmpeg_shared_libries ${FFMPEG_DEV_ROOT}/bin/*dll)
file(COPY ${ffmpeg_shared_libries} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)

add_subdirectory(src)
add_subdirectory(test)
