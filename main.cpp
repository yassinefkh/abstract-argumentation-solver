#include "Parser.h"
#include "utility.h"
#include "ArgumentationFramework.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 4 || (std::string(argv[2]) == "-p" && argc < 5)) {
        std::cerr << "Usage: " << argv[0] << " -p COMMAND -f FILE [-a ARG]\n";
        return 1;
    }

    // Parse command-line arguments
    std::string command = argv[2];
    std::string filename = argv[4];
    std::string argument = argc == 7 ? argv[6] : ""; // Optional argument for DC-XX and DS-XX

    try {
        // Parse the input file and build the argumentation framework
        Parser parser(filename);
        ArgumentationFramework af = parser.parse();
        if (command == "SE-CO") {
            
            #if DEBUG
                auto completeExtensions = af.enumerateCompleteExtensions();
                std::cout << "All complete extensions:\n";
                for (const auto& ext : completeExtensions) {
                    std::cout << formatExtension(ext) << "\n";
                }
            #endif
            auto completeExtension = af.findOneCompleteExtension();
            if (completeExtension.empty() && af.enumerateCompleteExtensions().empty()) {
                std::cout << "NO\n";
            } else {
                std::cout <<  formatExtension(completeExtension) << "\n";
            }
                    

        } else if (command == "DC-CO") {
            // DC-CO: Check credulous acceptance in complete extensions
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DC-CO.\n";
                return 1;
            }
            //std::cout << "Checking credulous acceptance (DC-CO) for argument '" << argument << "':\n";
            if (af.isCredulousComplete(argument)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }

        } else if (command == "DS-CO") {
            // DS-CO: Check skeptical acceptance in complete extensions
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DS-CO.\n";
                return 1;
            }
            //std::cout << "Checking skeptical acceptance (DS-CO) for argument '" << argument << "':\n";
            if (af.isSkepticalComplete(argument)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }

        } else if (command == "SE-ST") {
            // SE-ST: Give one stable extension
            //std::cout << "Computing one stable extension (SE-ST):\n";
            #if DEBUG
                auto stableExtensions = af.enumerateStableExtensions();
                std::cout << "All stable extensions:\n";
                for (const auto& ext : stableExtensions) {
                    std::cout << formatExtension(ext) << "\n";
                }
            #endif
           auto stableExtension = af.findOneStableExtension();
            if (!stableExtension.empty()) {
                std::cout << formatExtension(stableExtension) << "\n";
            } else {
                std::cout << "NO\n"; 
            }

        } else if (command == "DC-ST") {
            // DC-ST: Check credulous acceptance in stable extensions
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DC-ST.\n";
                return 1;
            }
            //std::cout << "Checking credulous acceptance (DC-ST) for argument '" << argument << "':\n";
            if (af.isCredulousStable(argument)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }

        } else if (command == "DS-ST") {
            // DS-ST: Check skeptical acceptance in stable extensions
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DS-ST.\n";
                return 1;
            }
            //std::cout << "Checking skeptical acceptance (DS-ST) for argument '" << argument << "':\n";
            if (af.isSkepticalStable(argument)) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }

        } else {
            std::cerr << "Error: Unknown command '" << command << "'.\n";
            return 1;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
