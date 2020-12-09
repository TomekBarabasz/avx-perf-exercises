//
// Created by barabasz on 09.12.2020.
//
#include <iostream>
#include <perf-measure.h>
#include <Modulate.h>

void generate_symbols(uint8_t* data, unsigned numSymbols)
{

}

int test_modulate(int argc, char** argv)
{
    if (argc < 2){
        std::cout << "valid arguments are: <num_symbols> <modulation_type> [generic|avx2|avx512|cuda]" << std::endl;
        return 1;
    }

    const unsigned numSymbols = std::atoi( argv[0] );
    const auto modulationType = argv[1];
    const auto implementationType = argc == 3 ? argv[2] : "generic";

    std::cout << "Modulate test function: numSymbols " << numSymbols << " modulation " << modulationType << " implementation " << implementationType << std::endl;

    auto mod = IModulate::createInstance(modulationType);
    const unsigned numBytes = (numSymbols + 7 ) / 8;
    uint8_t *inputData = mod->allocMem(numBytes);
    auto *outputData = reinterpret_cast<cint16_t*>( mod->allocMem(numSymbols) );

    generate_symbols(inputData,numSymbols);
    Measurements mm;
    mm.start();
    mod->modulate(inputData, outputData, numSymbols);
    std::cout << "execution time : " << mm.elapsed() << std::endl;
    mod->freeMem(inputData);
    mod->freeMem(reinterpret_cast<uint8_t*>(outputData));
    mod->release();
    return 0;
}
