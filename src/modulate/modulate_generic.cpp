//
// Created by barabasz on 09.12.2020.
//
#include <Modulate.h>
#include <string.h>

struct GenericAlloc : IModulate
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

struct GenericQPSK : GenericAlloc
{
    static constexpr int16_t QPSK_POS1 = 0x16A1;
    static constexpr int16_t QPSK_NEG1 = 0xE95F;
    static constexpr int16_t qpsk_lut[2] = {QPSK_POS1, QPSK_NEG1};
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {
        constexpr auto numSymbolsPerByte = 8 / 2;
        for (unsigned is=0; is<numSymbols; ++is){

        }

        uint8_t mask;
    }
};

struct Generic16QAM : GenericAlloc
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {

    }
};

struct Generic64QAM : GenericAlloc
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {

    }
};

struct Generic256QAM : GenericAlloc
{
    void modulate(uint8_t* dataIn, cint16_t* dataOut, unsigned numSymbols) override
    {

    }
};


IModulate* createGenericModulate(const char* type)
{
    if      (0==strcmp(type, "qpsk"))    return new GenericQPSK();
    else if (0==strcmp(type, "16qam"))   return new Generic16QAM();
    else if (0==strcmp(type, "64qam"))   return new Generic64QAM();
    else if (0==strcmp(type, "256qam"))  return new Generic256QAM();
    else                                 return new GenericQPSK();
}
