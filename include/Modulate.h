//
// Created by barabasz on 09.12.2020.
//
#ifndef PERF_TEST_MODULATE_H
#define PERF_TEST_MODULATE_H

#include <complex>

using cint16_t = std::complex<int16_t>;
using uint8_t = unsigned char;

struct IModulate
{
    enum { Generic=0, Generic1, Generic2, AVX, AVX1,AVX512, CUDA };
    static IModulate* createInstance(const char* type, unsigned hint=Generic);
    virtual uint8_t* allocMem(long long numElements) = 0;
    virtual void freeMem(uint8_t*) = 0;
    virtual void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) = 0;
    virtual void release() = 0;
protected:
    virtual ~IModulate() {};
};

#endif //PERF_TEST_MODULATE_H
