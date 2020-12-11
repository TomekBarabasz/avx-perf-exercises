//
// Created by barabasz on 09.12.2020.
//
#include <gtest/gtest.h>
#include <Modulate.h>

using namespace std::complex_literals;

const std::complex<int16_t> sym00 = cint16_t(0x16A1, 0x16A1);
const std::complex<int16_t> sym01 = cint16_t(0xE95F, 0x16A1);
const std::complex<int16_t> sym10 = cint16_t(0x16A1, 0xE95F);
const std::complex<int16_t> sym11 = cint16_t(0xE95F, 0xE95F);

TEST(GenericModulateTests, TestQPSK_1cw)
{
    auto mod = IModulate::createInstance("qpsk", IModulate::Generic);
    ASSERT_NE(mod, nullptr);

    uint8_t codeword;
    cint16_t symbol;

    codeword = 0;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, sym00);

    codeword = 1;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, sym01);

    codeword = 2;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, sym10);

    codeword = 3;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, sym11);

    mod->release();
}

TEST(GenericModulateTests, TestQPSK_5cw)
{
    auto mod = IModulate::createInstance("qpsk", IModulate::Generic);
    ASSERT_NE(mod, nullptr);

    uint8_t codewords[] = {0b11100100, 0b00000011};
    cint16_t symbols[6] = {0};

    mod->modulate(codewords, symbols, 5);
    ASSERT_EQ(symbols[0], sym00);
    ASSERT_EQ(symbols[1], sym01);
    ASSERT_EQ(symbols[2], sym10);
    ASSERT_EQ(symbols[3], sym11);
    ASSERT_EQ(symbols[4], sym11);
    ASSERT_EQ(symbols[5], cint16_t(0,0));

    mod->release();
}