add_library(modulate STATIC modulate.cpp modulate_generic.cpp modulate_avx.cpp)
target_include_directories(modulate PUBLIC
    ../../include
)

target_compile_options(modulate PRIVATE
    -O2
    -mavx2
    -mavx512f
)

