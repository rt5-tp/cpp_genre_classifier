cmake_minimum_required(VERSION 3.18.4)

project(Test)

set(SOURCES testGenrePredictor.cpp)
find_package( CURL REQUIRED )

add_executable(test ${SOURCES})

target_link_libraries(test gtest pthread CURL::libcurl)