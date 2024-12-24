#ifndef ARGUMENTATION_FRAMEWORK_H
#define ARGUMENTATION_FRAMEWORK_H

#include <set>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <functional> // Pour std::function utilisé dans les fonctions récursives

class ArgumentationFramework {
public:
    // ------------------------------
    // MÉTHODES DE MANIPULATION
    // ------------------------------
    void addArgument(const std::string& arg);
    void addAttack(const std::string& attacker, const std::string& target);

    // ------------------------------
    // MÉTHODES DE BASE
    // ------------------------------
    bool isConflictFree(const std::set<std::string>& extension) const;
    bool defends(const std::string& arg, const std::set<std::string>& extension) const;

    // ------------------------------
    // MÉTHODES TRADITIONNELLES
    // ------------------------------
    bool isAdmissible(const std::set<std::string>& extension) const;
    bool isComplete(const std::set<std::string>& extension) const;
    bool isStable(const std::set<std::string>& extension) const;

    // ------------------------------
    // MÉTHODES BASÉES SUR LA FONCTION CARACTÉRISTIQUE
    // ------------------------------
    std::set<std::string> characteristicFunction(const std::set<std::string>& S) const;

    // ------------------------------
    // ÉNUMÉRATIONS NAÏVES ET OPTIMISEES
    // ------------------------------
    std::vector<std::set<std::string>> enumerateCompleteExtensions(int& counter) const;
    std::vector<std::set<std::string>> enumerateStableExtensions(int& counter) const;

    

    // ------------------------------
    // RECHERCHE D'UNE EXTENSION (NAIVE ET OPTIMISEE)
    // ------------------------------
    std::set<std::string> findOneCompleteExtension(int& counter) const;
    std::set<std::string> findOneStableExtension(int& counter) const;
    std::set<std::string> findCompleteExtensionPlus(int& counter) const;
    std::set<std::string> findStableExtensionPlus(int& counter) const;

    // ------------------------------
    // TESTS DE CROYANCE
    // ------------------------------
    // Approches naïves
    bool isCredulousComplete(const std::string& argument, int& counter) const;
    bool isSkepticalComplete(const std::string& argument, int& counter) const;
    bool isCredulousStable(const std::string& argument, int& counter) const;
    bool isSkepticalStable(const std::string& argument, int& counter) const;

    // Approches fonction caractéristique
    bool isCredulousCompletePlus(const std::string& argument, int& counter) const;
    bool isSkepticalCompletePlus(const std::string& argument, int& counter) const;
    bool isCredulousStablePlus(const std::string& argument, int& counter) const;
    bool isSkepticalStablePlus(const std::string& argument, int& counter) const;

    // ------------------------------
    // LABELLING
    // ------------------------------
    std::unordered_map<std::string, std::string> labellingComplet(int& stateCounter) const;
    std::unordered_map<std::string, std::string> labellingStable(int& stateCounter) const;
    void displayLabelling(const std::unordered_map<std::string, std::string>& labels) const;
    

    // ------------------------------
    // MÉTHODES UTILITAIRES
    // ------------------------------
    void display() const;
    const std::vector<std::string>& getArguments() const;

private:
  
    bool backtrackForStableExtension(std::set<std::string> currentExtension,
                                     std::set<std::string>& stableExtension,
                                     std::set<std::string>& visitedArguments) const;

    std::unordered_map<std::string, int> argumentIndices;  // mapping argument/index
    std::vector<std::string> arguments;                    // liste des arguments
    std::vector<std::vector<bool>> attackMatrix;           // matrice des attaques
};

#endif // ARGUMENTATION_FRAMEWORK_H