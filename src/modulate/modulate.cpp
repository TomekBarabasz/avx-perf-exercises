//
// Created by barabasz on 09.12.2020.
//
#include <Modulate.h>
#include <stdlib.h>

IModulate* createGenericModulate(const char*,unsigned int hint);
IModulate* createAvxModulate(const char*);
IModulate* createAvx512Modulate(const char*);
IModulate* createCUDAModulate(const char*) { return nullptr;}

IModulate* IModulate::createInstance(const char *type, unsigned int hint) {
    switch (hint) {
    default:
    case Generic:   return createGenericModulate(type,  hint);
    case Generic1:   return createGenericModulate(type, hint);
    case Generic2:   return createGenericModulate(type, hint);
    case AVX:       return createAvxModulate(type);
    case AVX512:    return createAvx512Modulate(type);
    case CUDA:      return createCUDAModulate(type);
    }
}