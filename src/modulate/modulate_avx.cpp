//
// Created by barabasz on 09.12.2020.
//
#include <Modulate.h>
#include <string.h>

template <int ALIGN>
struct AVXAlloc : IModulate
{
    uint8_t* allocMem(unsigned numElements) override
    {
        return new uint8_t[numElements];
    }
    void freeMem(uint8_t* ptr) override
    {
        delete[] ptr;
    }
    void release() override
    {
        delete this;
    }
};

struct AVXQPSK : AVXAlloc<256>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

struct AVX16QAM : AVXAlloc<256>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

struct AVX64QAM : AVXAlloc<256>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

struct AVX256QAM : AVXAlloc<256>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

struct AVX512_QPSK : AVXAlloc<512>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

struct AVX512_16QAM : AVXAlloc<512>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

struct AVX512_64QAM : AVXAlloc<512>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

struct AVX512_256QAM : AVXAlloc<512>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
    }
};

IModulate* createAvxModulate(const char* type)
{
    if      (0==strcmp(type, "qpsk"))    return new AVXQPSK();
    else if (0==strcmp(type, "16qam"))   return new AVX16QAM();
    else if (0==strcmp(type, "64qam"))   return new AVX64QAM();
    else if (0==strcmp(type, "256qam"))  return new AVX256QAM();
    else                                 return new AVXQPSK();
}
IModulate* createAvx512Modulate(const char* type)
{
    if      (0==strcmp(type, "qpsk"))    return new AVX512_QPSK();
    else if (0==strcmp(type, "16qam"))   return new AVX512_16QAM();
    else if (0==strcmp(type, "64qam"))   return new AVX512_64QAM();
    else if (0==strcmp(type, "256qam"))  return new AVX512_256QAM();
    else                                 return new AVX512_QPSK();
}


