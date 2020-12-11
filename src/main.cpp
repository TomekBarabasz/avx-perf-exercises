#include <iostream>
#include <Modulate.h>
#include <map>
#include <string>

using test_fcn_t = int(*)(int,char**);
int test_modulate(int argc, char** argv);
int test_demodulate(int argc, char** argv);

void printValidModules(const std::map<std::string, test_fcn_t> &Modules) {
    std::cout << "valid modules are : ";
    for (auto kv : Modules) std::cout << kv.first << ", ";
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    std::map<std::string, test_fcn_t> Modules =
    {
        {"mod",    test_modulate},
        {"demod",  test_demodulate}
    };
    if (argc <2){
        printValidModules(Modules);
        return 1;
    }
    auto it = Modules.find(argv[1]);
    if (it == Modules.end()){
        printValidModules(Modules);
        return 1;
    }
    auto m = it->second;
    return m(argc - 2, argv + 2);
}
