//
// Created by barabasz on 09.12.2020.
//
#include <iostream>
#include <string.h>
#include <perf-measure.h>
#include <Modulate.h>
#include <random>
#include <cassert>

int getCodewordSize(const char *modType)
{
    int cwSize=2;
    if (0==strcmp(modType, "qpsk")) cwSize=2;
    else if (0==strcmp(modType, "16qam")) cwSize=4;
    else if (0==strcmp(modType, "64qam")) cwSize=6;
    else if (0==strcmp(modType, "256qam")) cwSize=8;
    return cwSize;
}

int implementationTypeToHint(const char *implType)
{
    int hint=IModulate::Generic;
    if (0==strcmp(implType,"generic")) hint=IModulate::Generic;
    else if (0==strcmp(implType,"generic1")) hint=IModulate::Generic1;
    else if (0==strcmp(implType,"generic2")) hint=IModulate::Generic2;
    else if (0==strcmp(implType,"avx")) hint=IModulate::AVX;
    else if (0==strcmp(implType,"avx512")) hint=IModulate::AVX512;
    else if (0==strcmp(implType,"cuda")) hint=IModulate::CUDA;
    return hint;
}

void generate_codewords(uint8_t* data, int numBytes)
{
    std::random_device dev;
    std::mt19937 gen(dev());

    const auto numBytesInWord = sizeof(std::mt19937::result_type);
    auto numWords = numBytes / numBytesInWord;
    while (--numWords > 0) {
        *reinterpret_cast<std::mt19937::result_type*>(data) = gen();
        data += numBytesInWord;
    }
    numBytes = numBytes % numBytesInWord;
    if (numBytes > 0)
    {
        auto lastDword = gen();
        while (--numBytes > 0) {
            *data++ = lastDword & 0xFF;
            lastDword >>= 8;
        }
    }
}

int test_modulate(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "valid arguments are: <num_codewords> [qpsk|16qam|64qam|256qam] [generic|avx|avx512|cuda]" << std::endl;
        return 1;
    }

    const unsigned numCodewords = std::atoi(argv[0] );
    const auto modulationType = argv[1];
    const auto implementationType = argc == 3 ? argv[2] : "generic";
    const auto cwSize = getCodewordSize(modulationType);
    std::cout << "Modulate test function: numCodewords " << numCodewords << " modulation " << modulationType << " implementation " << implementationType << std::endl;

    auto mod = IModulate::createInstance(modulationType, implementationTypeToHint(implementationType));
    const unsigned numBytes = (numCodewords * cwSize + 7 ) / 8;
    uint8_t *inputData = mod->allocMem(numBytes);
    auto *outputData = reinterpret_cast<cint16_t*>( mod->allocMem(numCodewords * sizeof(cint16_t)) );

    generate_codewords(inputData, numBytes);

    Measurements mm;
    mm.start();
    mod->modulate(inputData, outputData, numCodewords);
    std::cout << "execution time : " << mm.elapsed() << std::endl;
    mod->freeMem(inputData);
    mod->freeMem(reinterpret_cast<uint8_t*>(outputData));
    mod->release();
    return 0;
}
