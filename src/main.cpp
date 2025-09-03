#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "frontend/frontend.hpp"
#include "vm/backend.hpp"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Excellang, a language made by Infinity and TheDevConnor.\n";
        std::cout << "Type elg help for a list of commands\n";
        return 0;
    }

    std::string command = argv[1];
    std::unordered_map<std::string, std::function<void()>> switch_case = {
        {"help", []{
            std::cout << "Here is a list of commands that the Excellang terminal provides:\n";
            std::cout << "1. elg help: Returns a list of commands that the Excellang terminal provides.\n";
            std::cout << "2. elg clean: Clears the terminal.\n";
            std::cout << "3. elg authors: Displays the author of this project\n";
            std::cout << "4. elg build <src_file, without providing any file extension> <dest_file, without providing any file extension>: Builds up a custom VM file so that Excellang's VM can run\n";
        }},
        {"clean", []{
            std::cout << "\x1B[2J\x1B[H";
        }},
        {"authors", []{
            std::cout << "This project is made by Infinity and TheDevConnor.\n";
        }},
        {"build", [=]{
            if (argc < 4) {
                std::cout << "Not enough arguments for elg build!\n";
                return;
            }

            frontend(argv[2], argv[3]);
        }},
        {"run", [=]{
            if (argc < 3) {
                std::cout << "Not enough arguments for elg run!\n";
            }

            backend(argv[2]);
        }}
    };
    
    auto it = switch_case.find(command);
    if (it == switch_case.end()) {
        std::cout << "Invalid command\n";
    } else {
        it->second();
    }

    return 0;
}