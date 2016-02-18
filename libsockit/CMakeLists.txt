cmake_minimum_required(VERSION 2.8)
project(libsockit)

set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SOURCE_FILES "libsockit/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libsockit)

add_library(
    libsockit
    SHARED
    ${SOURCE_FILES}
)