#include "Parser.h"
#include "utility.h"
#include "ArgumentationFramework.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc < 4 || (std::string(argv[2]) == "-p" && argc < 5)) {
        std::cerr << "Usage: " << argv[0] << " -p COMMAND -f FILE [-a ARG]\n";
        return 1;
    }

    std::string command = argv[2];
    std::string filename = argv[4];
    std::string argument = argc == 7 ? argv[6] : "";

    try {
        Parser parser(filename);
        ArgumentationFramework af = parser.parse();

        if (command == "SE-CO") {
            int bruteForceCounter = 0;
            auto bruteForceExtension = af.findOneCompleteExtension(bruteForceCounter);
            if (!bruteForceExtension.empty()) {
                std::cout << "[";
                for (auto it = bruteForceExtension.begin(); it != bruteForceExtension.end(); ++it) {
                    if (it != bruteForceExtension.begin()) std::cout << ",";
                    std::cout << *it;
                }
                std::cout << "]\n";
            } else {
                std::cout << "NO\n";
            }
            //std::cout << "States explored: " << bruteForceCounter << "\n";



        } else if (command == "SE-ST") {
            int bruteForceCounter = 0;
            auto bruteForceExtension = af.findOneStableExtension(bruteForceCounter);
            if (!bruteForceExtension.empty()) {
                std::cout << "[";
                for (auto it = bruteForceExtension.begin(); it != bruteForceExtension.end(); ++it) {
                    if (it != bruteForceExtension.begin()) std::cout << ",";
                    std::cout << *it;
                }
                std::cout << "]\n";
            } else {
                std::cout << "NO\n";
            }
            std::cout << "States explored: " << bruteForceCounter << "\n";

        } else if (command == "DC-CO") {
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DC-CO.\n";
                return 1;
            }

            int bruteForceCounter = 0;
            bool resultBrute = af.isCredulousComplete(argument, bruteForceCounter);
            std::cout << (resultBrute ? "YES\n" : "NO\n");
            std::cout << "States explored: " << bruteForceCounter << "\n";

        } else if (command == "DS-CO") {
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DS-CO.\n";
                return 1;
            }

            int bruteForceCounter = 0;
            bool bruteForceResult = af.isSkepticalComplete(argument, bruteForceCounter);
            std::cout << (bruteForceResult ? "YES\n" : "NO\n");
            std::cout << "States explored: " << bruteForceCounter << "\n";

        } else if (command == "DC-ST") {
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DC-ST.\n";
                return 1;
            }

            int bruteForceCounter = 0;
            bool resultBrute = af.isCredulousStable(argument, bruteForceCounter);
            std::cout << (resultBrute ? "YES\n" : "NO\n");
            std::cout << "States explored: " << bruteForceCounter << "\n";

        } else if (command == "DS-ST") {
            if (argument.empty()) {
                std::cerr << "Error: Argument required for DS-ST.\n";
                return 1;
            }

            int bruteForceCounter = 0;
            bool resultBrute = af.isSkepticalStable(argument, bruteForceCounter);
            std::cout << (resultBrute ? "YES\n" : "NO\n");
            std::cout << "States explored: " << bruteForceCounter << "\n";

        } else {
            std::cerr << "Error: Unknown command '" << command << "'.\n";
            return 1;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
