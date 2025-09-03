#include "backend.hpp"
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include "vm.hpp"

void backend(const std::string vmfile) {
    std::ifstream vm_file(vmfile + ".elgb");
    if (!vm_file) {
        std::cout << "File " << vmfile << ".elgb cannot be found!\n";
        return;
    }

    std::string line;
    std::stringstream src;
    while (vm_file.is_open() && std::getline(vm_file, line)) {
        src << line << "\n";
    }

    VM* vm = new VM(src.str());
    vm->run();
}