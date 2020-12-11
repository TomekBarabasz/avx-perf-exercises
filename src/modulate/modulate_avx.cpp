//
// Created by barabasz on 09.12.2020.
//
#include <stdlib.h>
#include <Modulate.h>
#include <string.h>
#include <immintrin.h>
#define ALIGNED(x) __attribute__((aligned(x)))

template <int ALIGN>
struct AVXAlloc : IModulate
{
    uint8_t* allocMem(unsigned numElements) override
    {
        constexpr auto bytesInWord = ALIGN / 8;
        return reinterpret_cast<uint8_t*>( aligned_alloc(bytesInWord, numElements + (numElements % bytesInWord) * bytesInWord) );
    }
    void freeMem(uint8_t* ptr) override
    {
        free(ptr);
    }
    void release() override
    {
        delete this;
    }
};

struct AVXQPSK : AVXAlloc<256>
{
    static constexpr int16_t POS = 0x16A1;
    static constexpr int16_t NEG = 0xE95F;
    static constexpr /*ALIGNED(32)*/ cint16_t qpsk_lut[] = {
            cint16_t(POS, POS),
            cint16_t(NEG, POS),
            cint16_t(POS, NEG),
            cint16_t(NEG, NEG)
    };
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
        const __m256i all = _mm256_loadu2_m128i(reinterpret_cast<const __m128i *>(qpsk_lut),reinterpret_cast<const __m128i *>(qpsk_lut));
        const __m128i load_mask = _mm_setr_epi32 (1, 0, 0, 0);

        auto numWords = numSymbols / 8;
        if (numWords) {
            while(--numWords > 0)
            {
                __m128i cw1 = _mm_maskload_epi32 (reinterpret_cast<const int *>(dataIn), load_mask);
                __m256i cw2 = _mm256_broadcastd_epi32 (cw1);
            }
        }

        for (numSymbols = numSymbols % 8;numSymbols>0;--numSymbols)
        {

        }
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


