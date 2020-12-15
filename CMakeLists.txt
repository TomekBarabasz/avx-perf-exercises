cmake_minimum_required(VERSION 3.16)
project(perf_test)

set(CMAKE_CXX_STANDARD 17)

set(MODULES
    ./src/TestModulate.cpp
    ./src/TestDemodulate.cpp
        include/common.h)

add_executable(perf_test src/main.cpp ${MODULES})
add_subdirectory(src/modulate)
add_subdirectory(test)
target_include_directories(perf_test PUBLIC
    ./include
    ../oneTBB/include
)
target_link_libraries(perf_test
        stdc++
        modulate
)
target_compile_options(perf_test PRIVATE
        -O2
)