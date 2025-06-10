cmake_minimum_required(VERSION 3.15)
project(cue-api-runner LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE
    "_path_/_to_/_vcpkg_/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

set(VCPKG_MANIFEST_MODE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cue-api-runner
    src/main.cpp
    src/Request.cpp
#    src/RequestLoader.cpp
 "src/Request.cpp" "src/RequestLoader.cpp")
target_include_directories(cue-api-runner PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_package(CURL    REQUIRED)
find_package(yaml-cpp REQUIRED)

target_link_libraries(cue-api-runner PRIVATE
    CURL::libcurl
    yaml-cpp
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()