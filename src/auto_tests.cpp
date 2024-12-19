#include <random>
#include <fstream>
#include <iostream>
#include <csignal> 
#include <chrono> 
#include "ArgumentationFramework.h"

// variables globales pour enregistrer les résultats partiels
std::vector<std::vector<std::string>> results;
std::string outputFilename = "results.csv";

// Fonction pour sauvegarder les résultats partiels
void savePartialResults() {
    std::cerr << "Saving partial results to " << outputFilename << "...\n";
    std::ofstream file(outputFilename);

    if (file.is_open()) {
        for (const auto& row : results) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
        std::cerr << "Partial results saved successfully.\n";
    } else {
        std::cerr << "Unable to open file for writing partial results.\n";
    }
}

// gestionnaire pour SIGSEGV et SIGINT
void signalHandler(int signal) {
    if (signal == SIGSEGV) {
        std::cerr << "Error: Segmentation fault detected. ";
    } else if (signal == SIGINT) {
        std::cerr << "Program interrupted (Ctrl+C). ";
    }
    savePartialResults();
    exit(signal);
}

// génération de graphes aléatoires
ArgumentationFramework generateRandomGraph(int numArguments, float density) {
    ArgumentationFramework af;
    std::vector<std::string> arguments;

    // ajouter des arguments
    for (int i = 0; i < numArguments; ++i) {
        std::string arg = "A" + std::to_string(i);
        af.addArgument(arg);
        arguments.push_back(arg);
    }
    //std::cout << "Arguments added: " << numArguments << std::endl;

    // ajouter des attaques aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);

    int attackCount = 0;
    for (const auto& attacker : arguments) {
        for (const auto& target : arguments) {
            if (attacker != target && dist(gen) < density) {
                af.addAttack(attacker, target);
                attackCount++;
            }
        }
    }
    std::cout << "Generated " << attackCount << " attacks." << std::endl;

    return af;
}

int main() {
   
    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGINT, signalHandler);

    results.push_back({"GraphSize", "NumAttacks", "Command", "NaiveStates", "CharFuncStates", "NaiveTime(ms)", "CharFuncTime(ms)"});

    for (int size = 4; size <= 50; size += 2) { // tester avec des tailles de graphes croissantes
        std::cout << "Generating graph with size: " << size << std::endl;
        for (float density = 0.2; density <= 0.8; density += 0.2) { // densité d'attaques croissante
            std::cout << "Using density: " << density << std::endl;
            try {
                ArgumentationFramework af = generateRandomGraph(size, density);

                for (const std::string& command : {"DC-CO", "DS-CO", "SE-CO", "DC-ST", "DS-ST", "SE-ST"}) {
                    //std::cout << "Executing command: " << command << std::endl;
                    int naiveCounter = 0, charFuncCounter = 0;
                    double naiveTime = 0.0, charFuncTime = 0.0;

                    // calcul du temps pour l'approche naïve
                    auto startNaive = std::chrono::high_resolution_clock::now();
                    if (command == "DC-CO") af.isCredulousComplete("A0", naiveCounter);
                    else if (command == "DS-CO") af.isSkepticalComplete("A0", naiveCounter);
                    else if (command == "SE-CO") af.findOneCompleteExtension(naiveCounter);
                    else if (command == "DC-ST") af.isCredulousStable("A0", naiveCounter);
                    else if (command == "DS-ST") af.isSkepticalStable("A0", naiveCounter);
                    else if (command == "SE-ST") af.findOneStableExtension(naiveCounter);
                    auto endNaive = std::chrono::high_resolution_clock::now();
                    naiveTime = std::chrono::duration<double, std::milli>(endNaive - startNaive).count();

                    // calcul du temps pour l'approche optimisée avec fonction caractéristique
                    auto startCharFunc = std::chrono::high_resolution_clock::now();
                    if (command == "DC-CO") af.isCredulousCompletePlus("A0", charFuncCounter);
                    else if (command == "DS-CO") af.isSkepticalCompletePlus("A0", charFuncCounter);
                    else if (command == "SE-CO") af.findCompleteExtensionPlus(charFuncCounter);
                    else if (command == "DC-ST") af.isCredulousStablePlus("A0", charFuncCounter);
                    else if (command == "DS-ST") af.isSkepticalStablePlus("A0", charFuncCounter);
                    else if (command == "SE-ST") af.findStableExtensionPlus(charFuncCounter);
                    auto endCharFunc = std::chrono::high_resolution_clock::now();
                    charFuncTime = std::chrono::duration<double, std::milli>(endCharFunc - startCharFunc).count();

                    // enregistrer les résultats
                    results.push_back({
                        std::to_string(size),
                        std::to_string(static_cast<int>(size * (size - 1) * density)),
                        command,
                        std::to_string(naiveCounter),
                        std::to_string(charFuncCounter),
                        std::to_string(naiveTime),
                        std::to_string(charFuncTime)
                    });
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << " during execution." << std::endl;
                continue; // passer au graphe suivant en cas d'erreur
            }
        }
    }

    savePartialResults();
    std::cout << "All results saved to " << outputFilename << std::endl;

    return 0;
}



/*

g++ -std=c++17 -o auto_tests src/auto_tests.cpp src/ArgumentationFramework.cpp src/utility.cpp -I include

*/