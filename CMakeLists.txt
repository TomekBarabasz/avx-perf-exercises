cmake_minimum_required(VERSION 3.17)
project(perf_test)

set(CMAKE_CXX_STANDARD 17)

set(MODULES
    ./src/Modulate.cpp
    ./src/Demodulate.cpp
        include/common.h)

add_executable(perf_test src/main.cpp ${MODULES})
add_subdirectory(src/modulate)
add_subdirectory(test)
target_include_directories(perf_test PUBLIC
    ./include
)
target_link_libraries(perf_test
        stdc++
        modulate
)
target_compile_options(perf_test PRIVATE
        -O2
)