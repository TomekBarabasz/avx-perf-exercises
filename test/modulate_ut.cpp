//
// Created by barabasz on 09.12.2020.
//
#include <gtest/gtest.h>
#include <Modulate.h>

using namespace std::complex_literals;

TEST(GenericModulateTests, TestQPSK_1symbol)
{
    auto mod = IModulate::createInstance("qpsk", IModulate::Generic);
    ASSERT_NE(mod, nullptr);

    uint8_t codeword;
    cint16_t symbol;

    codeword = 0;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, cint16_t(0x16A1,0x16A1));

    codeword = 1;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, cint16_t(0x16A1,0x16A1));

    codeword = 2;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, cint16_t(0x16A1,0x16A1));

    codeword = 3;
    mod->modulate(&codeword, &symbol, 1);
    ASSERT_EQ(symbol, cint16_t(0x16A1,0x16A1));

    mod->release();
}
