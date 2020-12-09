add_library(modulate STATIC modulate.cpp modulate_generic.cpp modulate_avx.cpp)
target_include_directories(modulate PUBLIC
    ../../include
)