//
// Created by barabasz on 09.12.2020.
//
#include <stdlib.h>
#include <Modulate.h>
#include <string.h>
#include <immintrin.h>
#include <iostream>
#define ALIGNED(x) __attribute__((aligned(x)))

#ifdef _WIN64
#include <malloc.h>
uint8_t* aligned_alloc(size_t alignement, size_t size)
{
    return reinterpret_cast<uint8_t*>(_aligned_malloc(size, alignement));
}
void aligned_free(void* ptr)
{
    _aligned_free(ptr);
}
#endif
#ifdef __linux__
void aligned_free(void* ptr)
{
    free(ptr);
}
#endif

template <int ALIGN>
struct AVXAlloc : IModulate
{
    uint8_t* allocMem(long long numElements) override
    {
        constexpr auto bytesInWord = ALIGN / 8;
        //return reinterpret_cast<uint8_t*>(aligned_alloc(bytesInWord, numElements + (numElements % bytesInWord) * bytesInWord) );
        return reinterpret_cast<uint8_t*>(aligned_alloc(bytesInWord, numElements + (numElements % bytesInWord) * bytesInWord) );
    }
    void freeMem(uint8_t* ptr) override
    {
        aligned_free(ptr);
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
    static constexpr ALIGNED(32) cint16_t qpsk_lut[] = {
            cint16_t(POS, POS),
            cint16_t(NEG, POS),
            cint16_t(POS, NEG),
            cint16_t(NEG, NEG)
    };
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
        const __m256i all = _mm256_loadu2_m128i(reinterpret_cast<const __m128i *>(qpsk_lut),reinterpret_cast<const __m128i *>(qpsk_lut));
        //const __m128i lut = _mm_load_si128(reinterpret_cast<const __m128i *>(qpsk_lut));
        //const __m256i all = _mm256_broadcastsi128_si256(lut);

        const __m256i shift = _mm256_setr_epi32 (0,2,4,6,8,10,12,14);
        const __m256i mask = _mm256_set1_epi32(3);

        auto pout = reinterpret_cast<__m256i*>(dataOut);
        auto numWords = numSymbols / 8;
        auto ending = numSymbols % 8;
        if (ending) ++numWords;
        while(numWords-- > 0)
        {
            __m256i cw2 = _mm256_set1_epi32( *reinterpret_cast<short*>(dataIn) );
            __m256i cw3 = _mm256_srav_epi32(cw2, shift);
            cw3 = _mm256_and_si256(cw3, mask);
            __m256i symbols = _mm256_permutevar8x32_epi32(all, cw3);
            if (numWords > 0 || 0 == ending)
            {
                _mm256_store_si256(pout, symbols);
                pout++;
                dataIn += 2;
            }else
            {
                __m256i m = _mm256_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7);
                const __m256i e = _mm256_set1_epi32((int)ending);
                m = _mm256_sub_epi32(m, e);
                _mm256_maskstore_epi32(reinterpret_cast<int*>(pout), m, symbols);
            }
        }
    }
};

struct AVX16QAM : AVXAlloc<256>
{
    static constexpr int16_t POS = 0x16A1;
    static constexpr int16_t NEG = 0xE95F;
    static constexpr ALIGNED(32) cint16_t qpsk_lut[] = {
            cint16_t(POS, POS),
            cint16_t(NEG, POS),
            cint16_t(POS, NEG),
            cint16_t(NEG, NEG)
    };
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
        const __m128i lut = _mm_load_si128(reinterpret_cast<const __m128i *>(qpsk_lut));

    }
};

struct AVX64QAM : AVXAlloc<256>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
    }
};

struct AVX256QAM : AVXAlloc<256>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
    }
};

struct AVX512_QPSK : AVXAlloc<512>
{
    static constexpr int16_t POS = 0x16A1;
    static constexpr int16_t NEG = 0xE95F;
    static constexpr ALIGNED(32) cint16_t qpsk_lut[] = {
            cint16_t(POS, POS),
            cint16_t(NEG, POS),
            cint16_t(POS, NEG),
            cint16_t(NEG, NEG)
    };
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
        auto pout = reinterpret_cast<__m256i*>(dataOut);
        auto numWords = numSymbols / 8;
        auto ending = numSymbols % 8;
        if (ending) ++numWords;
        if (numWords) {
            while (numWords-- > 0)
            {
                __m512i cw1 = _mm512_set1_epi32(*reinterpret_cast<const int*>(dataIn));
            }
        }
    }
};

struct AVX512_16QAM : AVXAlloc<512>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
    }
};

struct AVX512_64QAM : AVXAlloc<512>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
    }
};

struct AVX512_256QAM : AVXAlloc<512>
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
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


