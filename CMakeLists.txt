cmake_minimum_required(VERSION 3.15)

project(raytracing_in_one_week
    LANGUAGES CXX C
)

file(GLOB_RECURSE source include/*.h src/*.cpp)

add_executable(run_it ${source})

target_include_directories(run_it PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
