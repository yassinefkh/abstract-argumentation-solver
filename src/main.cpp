#include "Parser.h"
#include "utility.h"
#include "ArgumentationFramework.h"
#include <iostream>
#include <set>
#include <string>
#include <vector>

// decommentez pour activer le mode debug (affichage des informations de debug) 
// #define DEBUG

void displayLabelling(const std::unordered_map<std::string, std::string>& labelling) {
    std::cout << "Labelling:\n";
    for (const auto& [arg, label] : labelling) {
        std::cout << arg << ": " << label << "\n";
    }
}


int main(int argc, char *argv[])
{
    if (argc < 4 || (std::string(argv[2]) == "-p" && argc < 5))
    {
        std::cerr << "Usage: " << argv[0] << " -p COMMAND -f FILE [-a ARG]\n";
        return 1;
    }

    std::string command = argv[2];
    std::string filename = argv[4];
    std::string argument = argc == 7 ? argv[6] : "";

    try
    {
        Parser parser(filename);
        ArgumentationFramework af = parser.parse();

        if (command == "DC-CO")
        {
            if (argument.empty())
            {
                std::cerr << "Error: Argument required for DC-CO.\n";
                return 1;
            }

            int bruteForceCounter = 0, characteristicCounter = 0;

            bool resultBrute = af.isCredulousComplete(argument, bruteForceCounter);
            std::cout << "Brute-Force Result: " << (resultBrute ? "YES" : "NO") << "\n";
            std::cout << "States explored (Brute-Force): " << bruteForceCounter << "\n";

            bool resultCharacteristic = af.isCredulousCompletePlus(argument, characteristicCounter);
            std::cout << "Characteristic Function Result: " << (resultCharacteristic ? "YES" : "NO") << "\n";
            std::cout << "States explored (Characteristic): " << characteristicCounter << "\n";
        }
        else if (command == "DS-CO")
        {
            if (argument.empty())
            {
                std::cerr << "Error: Argument required for DS-CO.\n";
                return 1;
            }

            int bruteForceCounter = 0, characteristicCounter = 0;

            bool bruteForceResult = af.isSkepticalComplete(argument, bruteForceCounter);
            std::cout << "Brute-Force Result: " << (bruteForceResult ? "YES" : "NO") << "\n";
            std::cout << "States explored (Brute-Force): " << bruteForceCounter << "\n";

            bool characteristicResult = af.isSkepticalCompletePlus(argument, characteristicCounter);
            std::cout << "Characteristic Function Result: " << (characteristicResult ? "YES" : "NO") << "\n";
            std::cout << "States explored (Characteristic): " << characteristicCounter << "\n";
        }
        else if (command == "SE-CO")
        {
            int bruteForceCounter = 0, characteristicCounter = 0;

            auto bruteForceExtension = af.findOneCompleteExtension(bruteForceCounter);
            std::cout << "Brute-Force Extension: " << formatExtension(bruteForceExtension) << "\n";
            std::cout << "States explored (Brute-Force): " << bruteForceCounter << "\n";

            auto characteristicExtension = af.findCompleteExtensionPlus(characteristicCounter);
            std::cout << "Characteristic Function Extension: " << formatExtension(characteristicExtension) << "\n";
            std::cout << "States explored (Characteristic): " << characteristicCounter << "\n";

            #ifdef DEBUG
            auto completeLabelling = af.computeCompleteLabelling();
            displayLabelling(completeLabelling);
            #endif
        }
        else if (command == "DC-ST")
        {
            if (argument.empty())
            {
                std::cerr << "Error: Argument required for DC-ST.\n";
                return 1;
            }

            int bruteForceCounter = 0, characteristicCounter = 0;

            bool resultBrute = af.isCredulousStable(argument, bruteForceCounter);
            std::cout << "Brute-Force Result: " << (resultBrute ? "YES" : "NO") << "\n";
            std::cout << "States explored (Brute-Force): " << bruteForceCounter << "\n";

            bool resultCharacteristic = af.isCredulousStablePlus(argument, characteristicCounter);
            std::cout << "Characteristic Function Result: " << (resultCharacteristic ? "YES" : "NO") << "\n";
            std::cout << "States explored (Characteristic): " << characteristicCounter << "\n";
        }
        else if (command == "DS-ST")
        {
            if (argument.empty())
            {
                std::cerr << "Error: Argument required for DS-ST.\n";
                return 1;
            }

            int bruteForceCounter = 0, characteristicCounter = 0;

            bool resultBrute = af.isSkepticalStable(argument, bruteForceCounter);
            std::cout << "Brute-Force Result: " << (resultBrute ? "YES" : "NO") << "\n";
            std::cout << "States explored (Brute-Force): " << bruteForceCounter << "\n";

            bool resultCharacteristic = af.isSkepticalStablePlus(argument, characteristicCounter);
            std::cout << "Characteristic Function Result: " << (resultCharacteristic ? "YES" : "NO") << "\n";
            std::cout << "States explored (Characteristic): " << characteristicCounter << "\n";
        }
        else if (command == "SE-ST")
        {
            int bruteForceCounter = 0, characteristicCounter = 0;

            // approche brute-force
            auto bruteForceExtension = af.findOneStableExtension(bruteForceCounter);
            std::cout << "Brute-Force Extension: " << formatExtension(bruteForceExtension) << "\n";
            std::cout << "States explored (Brute-Force): " << bruteForceCounter << "\n";

            // version avec fonction caractèristique
            auto characteristicExtension = af.findStableExtensionPlus(characteristicCounter);
            std::cout << "Characteristic Function Extension: " << formatExtension(characteristicExtension) << "\n";
            std::cout << "States explored (Characteristic): " << characteristicCounter << "\n";

             #ifdef DEBUG
            auto stableLabelling = af.computeStableLabelling();
            if (!stableLabelling.empty()) {
                displayLabelling(stableLabelling);
            } else {
                std::cout << "No stable labelling found.\n";
            }
            #endif
        }
        else
        {
            std::cerr << "Error: Unknown command '" << command << "'.\n";
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
