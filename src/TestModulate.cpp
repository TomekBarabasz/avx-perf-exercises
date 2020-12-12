//
// Created by barabasz on 09.12.2020.
//
#include <iostream>
#include <string.h>
#include <perf-measure.h>
#include <Modulate.h>
#include <random>
#include <cassert>
#include <map>
#include <string>
#include <thread>
#include <ppl.h>

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
    std::map<std::string, int> Hints =
    {
        {"gen",     IModulate::Generic},
        {"gen-1",   IModulate::Generic1},
        {"gen-2",   IModulate::Generic2},
        {"avx",     IModulate::AVX},
        {"avx-1",   IModulate::AVX1},
        {"avx512",  IModulate::AVX512},
        {"cuda",    IModulate::CUDA}
    };
    auto it = Hints.find(implType);
    if (it != Hints.end()) return it->second;
    else return IModulate::Generic;
}

void generate_codewords(uint8_t* data, long long numBytes)
{
    using namespace concurrency;
    const int Nthr = std::thread::hardware_concurrency();
    parallel_for(1, Nthr, [=](int tid) 
    {
        std::random_device dev;
        std::mt19937 gen(dev());

        auto thread_num_bytes = numBytes / Nthr;
        auto tdata = data + (tid-1) * thread_num_bytes;
        if (tid == Nthr - 1) {
            thread_num_bytes += numBytes % Nthr;
        }
        const auto numBytesInWord = sizeof(std::mt19937::result_type);
        auto numWords = thread_num_bytes / numBytesInWord;
        while (numWords-- > 0) {
            *reinterpret_cast<std::mt19937::result_type*>(tdata) = gen();
            tdata += numBytesInWord;
        }
        thread_num_bytes = thread_num_bytes % numBytesInWord;
        if (thread_num_bytes > 0)
        {
            auto lastDword = gen();
            while (thread_num_bytes-- > 0) {
                *tdata++ = lastDword & 0xFF;
                lastDword >>= 8;
            }
        }
    });
}

int test_modulate(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "valid arguments are: <num_codewords> [qpsk|16qam|64qam|256qam] [generic|avx|avx512|cuda]" << std::endl;
        return 1;
    }

    const auto numCodewords = std::atoll(argv[0] );
    const auto modulationType = argv[1];
    const auto implementationType = argc == 3 ? argv[2] : "generic";
    const auto cwSize = getCodewordSize(modulationType);
    std::cout << "Modulate test function: numCodewords " << numCodewords << " modulation " << modulationType << " implementation " << implementationType << std::endl;

    auto mod = IModulate::createInstance(modulationType, implementationTypeToHint(implementationType));
    const auto numBytes = (numCodewords * cwSize + 7 ) / 8;
    uint8_t *inputData = mod->allocMem(numBytes);
    auto *outputData = reinterpret_cast<cint16_t*>( mod->allocMem(numCodewords * sizeof(cint16_t)) );
    
    Measurements mm;
    mm.start();
    std::cout << "generating random codewords ... ";
    generate_codewords(inputData, numBytes);
    std::cout << "done in " << mm.elapsed() << std::endl;
    
    mm.start();
    mod->modulate(inputData, outputData, numCodewords);
    std::cout << "execution time : " << mm.elapsed() << std::endl;
    mod->freeMem(inputData);
    mod->freeMem(reinterpret_cast<uint8_t*>(outputData));
    mod->release();
    return 0;
}
