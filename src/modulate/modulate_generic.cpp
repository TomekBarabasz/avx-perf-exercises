//
// Created by barabasz on 09.12.2020.
//
#include <Modulate.h>
#include <string.h>

struct GenericAlloc : IModulate
{
    uint8_t* allocMem(long long numElements) override
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

static constexpr int16_t QPSK_POS1 = 0x16A1;    //1/sqrt(2)
static constexpr int16_t QPSK_NEG1 = 0xE95F;
static constexpr int16_t qpsk_lut_short[] = {QPSK_POS1, QPSK_NEG1};
static constexpr cint16_t qpsk_lut_long[] = {
        cint16_t(QPSK_POS1, QPSK_POS1),
        cint16_t(QPSK_NEG1, QPSK_POS1),
        cint16_t(QPSK_POS1, QPSK_NEG1),
        cint16_t(QPSK_NEG1, QPSK_NEG1)
};

struct GenericQPSK : GenericAlloc
{
    static constexpr int BitsPerCodeword = 2;
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
        int shift = 0;
        int16_t *pout = reinterpret_cast<int16_t*>(dataOut);
        for (long long is=0; is<numSymbols; ++is)
        {
            const auto codeword = *dataIn >> shift;
            *pout++ = qpsk_lut_short[codeword & 1 ? 1 : 0];
            *pout++ = qpsk_lut_short[codeword & 2 ? 1 : 0];
            shift += BitsPerCodeword;
            if (8 == shift){
                shift = 0;
                ++dataIn;
            }
        }
    }
};

struct Generic1QPSK : GenericAlloc
{
    static constexpr int BitsPerCodeword = 2;
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
        int shift = 0;
        for (long long is=0; is<numSymbols; ++is)
        {
            const auto codeword = *dataIn >> shift;
            *dataOut++ = qpsk_lut_long[codeword & 3];
            shift += BitsPerCodeword;
            if (8 == shift){
                shift = 0;
                ++dataIn;
            }
        }
    }
};

struct Generic2QPSK : GenericAlloc
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {
    }
};

struct Generic16QAM : GenericAlloc
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {

    }
};

struct Generic64QAM : GenericAlloc
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {

    }
};

struct Generic256QAM : GenericAlloc
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, long long numSymbols) override
    {

    }
};

IModulate* createGenericModulate(const char* type, unsigned int hint)
{
    if (0==strcmp(type, "qpsk"))    {
        switch(hint){
            default:
            case IModulate::Generic:    return new GenericQPSK();
            case IModulate::Generic1:   return new Generic1QPSK();
            case IModulate::Generic2:   return new Generic2QPSK();
        }
    }
    else if (0==strcmp(type, "16qam"))   return new Generic16QAM();
    else if (0==strcmp(type, "64qam"))   return new Generic64QAM();
    else if (0==strcmp(type, "256qam"))  return new Generic256QAM();
    else                                 return new GenericQPSK();
}
