#include "ArgumentationFramework.h"
#include "utility.h"
#include <cstdlib>
#include <ctime>
#include <map>
#include <stdexcept>
#include <algorithm>

// ------------------------------------
// AJOUT D'ARGUMENTS ET ATTAQUES
// ------------------------------------

void ArgumentationFramework::addArgument(const std::string& arg) {
    if (argumentIndices.find(arg) == argumentIndices.end()) {
        argumentIndices[arg] = arguments.size();
        arguments.push_back(arg);
        for (auto& row : attackMatrix) {
            row.push_back(false);
        }
        attackMatrix.emplace_back(arguments.size(), false);
    }
}

void ArgumentationFramework::addAttack(const std::string& attacker, const std::string& target) {
    if (argumentIndices.find(attacker) == argumentIndices.end() || argumentIndices.find(target) == argumentIndices.end()) {
        throw std::runtime_error("Arguments not defined.");
    }
    int attackerIndex = argumentIndices[attacker];
    int targetIndex = argumentIndices[target];
    attackMatrix[attackerIndex][targetIndex] = true;
}

const std::vector<std::string>& ArgumentationFramework::getArguments() const {
    return arguments;
}

// ------------------------------------
// AFFICHAGE DU GRAPHE
// ------------------------------------

void ArgumentationFramework::display() const {
    std::cout << "Arguments: ";
    for (const auto& arg : arguments) std::cout << arg << " ";
    std::cout << "\nAttacks:\n";
    for (size_t i = 0; i < arguments.size(); ++i) {
        for (size_t j = 0; j < arguments.size(); ++j) {
            if (attackMatrix[i][j]) std::cout << arguments[i] << " -> " << arguments[j] << "\n";
        }
    }
}

// ------------------------------------
// OPERATIONS DE BASE
// ------------------------------------

bool ArgumentationFramework::isConflictFree(const std::set<std::string>& extension) const {
    for (const auto& arg1 : extension) {
        for (const auto& arg2 : extension) {
            if (attackMatrix[argumentIndices.at(arg1)][argumentIndices.at(arg2)]) {
                return false;
            }
        }
    }
    return true;
}

bool ArgumentationFramework::defends(const std::string& arg, const std::set<std::string>& extension) const {
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (attackMatrix[i][argumentIndices.at(arg)]) {
            bool defended = false;
            for (const auto& defender : extension) {
                if (attackMatrix[argumentIndices.at(defender)][i]) {
                    defended = true;
                    break;
                }
            }
            if (!defended) return false;
        }
    }
    return true;
}

bool ArgumentationFramework::isAdmissible(const std::set<std::string>& extension) const {
    return isConflictFree(extension) && std::all_of(extension.begin(), extension.end(),
        [this, &extension](const std::string& arg) { return defends(arg, extension); });
}

bool ArgumentationFramework::isComplete(const std::set<std::string>& extension) const {
    if (!isAdmissible(extension)) return false;
    for (const auto& arg : arguments) {
        if (defends(arg, extension) && extension.find(arg) == extension.end()) {
            return false;
        }
    }
    return true;
}

bool ArgumentationFramework::isStable(const std::set<std::string>& extension) const {
    if (!isConflictFree(extension)) return false;
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (extension.find(arguments[i]) == extension.end()) {
            bool attacked = false;
            for (const auto& attacker : extension) {
                if (attackMatrix[argumentIndices.at(attacker)][i]) {
                    attacked = true;
                    break;
                }
            }
            if (!attacked) return false;
        }
    }
    return true;
}




// ------------------------------------
// ENUMERATION NAIVE DES EXTENSIONS
// ------------------------------------

std::vector<std::set<std::string>> ArgumentationFramework::enumerateCompleteExtensions(int& counter) const {
    counter = 0;
    std::vector<std::set<std::string>> extensions;
    size_t n = arguments.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        counter++;
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) subset.insert(arguments[j]);
        }
        if (isComplete(subset)) {
            extensions.push_back(subset);
        }
    }
    return extensions;
}

std::vector<std::set<std::string>> ArgumentationFramework::enumerateCompleteExtensionsWithEarlyStop(
    const std::string& targetArg, int& counter, bool& found) const {
    counter = 0;
    std::vector<std::set<std::string>> extensions;
    size_t n = arguments.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        counter++;
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) subset.insert(arguments[j]);
        }
        if (isComplete(subset)) {
            extensions.push_back(subset);
            if (subset.find(targetArg) != subset.end()) {
                found = true;
                return extensions; // Arrêt immédiat si une extension contenant `targetArg` est trouvée
            }
        }
    }
    found = false;
    return extensions;
}


std::vector<std::set<std::string>> ArgumentationFramework::enumerateStableExtensions(int& counter) const {
    counter = 0;
    std::vector<std::set<std::string>> extensions;
    size_t n = arguments.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        counter++;
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) subset.insert(arguments[j]);
        }
        if (isStable(subset)) {
            extensions.push_back(subset);
        }
    }
    return extensions;
}

std::vector<std::set<std::string>> ArgumentationFramework::enumerateStableExtensionsWithEarlyStop(
    const std::string& targetArg, int& counter, bool& found) const {
    counter = 0;
    std::vector<std::set<std::string>> extensions;
    size_t n = arguments.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        counter++;
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) subset.insert(arguments[j]);
        }
        if (isStable(subset)) {
            extensions.push_back(subset);
            if (subset.find(targetArg) != subset.end()) {
                found = true;
                return extensions; // Arrêt immédiat si une extension contenant `targetArg` est trouvée
            }
        }
    }
    found = false;
    return extensions;
}


/* std::set<std::string> ArgumentationFramework::findOneCompleteExtension(int& counter) const {
    auto extensions = enumerateCompleteExtensions(counter);
    if (!extensions.empty()) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        return extensions[std::rand() % extensions.size()];
    }
    return {};
} */


/* std::set<std::string> ArgumentationFramework::findOneStableExtension(int& counter) const {
    auto extensions = enumerateStableExtensions(counter);
    if (!extensions.empty()) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        return extensions[std::rand() % extensions.size()];
    }
    return {};
} */

std::set<std::string> ArgumentationFramework::findOneStableExtension(int& counter) const {
    return findOneStableExtensionWithEarlyStop(counter);
}

std::set<std::string> ArgumentationFramework::findOneStableExtensionWithEarlyStop(int& counter) const {
    counter = 0;
    size_t n = arguments.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        counter++;
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) subset.insert(arguments[j]);
        }
        if (isStable(subset)) {
            return subset; // Retourne immédiatement la première extension stable trouvée
        }
    }
    return {}; // Retourne un ensemble vide si aucune extension stable n'est trouvée
}

std::set<std::string> ArgumentationFramework::findOneCompleteExtensionWithEarlyStop(int& counter) const {
    counter = 0;
    size_t n = arguments.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        counter++;
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) subset.insert(arguments[j]);
        }
        if (isComplete(subset)) {
            return subset; // Retourne immédiatement la première extension complète trouvée
        }
    }
    return {}; // Retourne un ensemble vide si aucune extension complète n'est trouvée
}

std::set<std::string> ArgumentationFramework::findOneCompleteExtension(int& counter) const {
    return findOneCompleteExtensionWithEarlyStop(counter);
}



// ------------------------------------
// APPROCHE NAIVE : CREDULOUS & SKEPTICAL
// ------------------------------------

bool ArgumentationFramework::isCredulousComplete(const std::string& argument, int& counter) const {
    counter = 0;
    auto extensions = enumerateCompleteExtensions(counter);
    for (const auto& ext : extensions) {
        if (ext.find(argument) != ext.end()) return true;
    }
    return false;
}

bool ArgumentationFramework::isCredulousCompleteEarlyStop(const std::string& argument, int& counter) const {
    bool found = false;
    enumerateCompleteExtensionsWithEarlyStop(argument, counter, found);
    return found;
}


bool ArgumentationFramework::isSkepticalComplete(const std::string& argument, int& counter) const {
    counter = 0;
    auto extensions = enumerateCompleteExtensions(counter);
    for (const auto& ext : extensions) {
        if (ext.find(argument) == ext.end()) return false;
    }
    return true;
}

bool ArgumentationFramework::isCredulousStable(const std::string& argument, int& counter) const {
    counter = 0;
    auto extensions = enumerateStableExtensions(counter);
    for (const auto& ext : extensions) {
        if (ext.find(argument) != ext.end()) return true;
    }
    return false;
}

bool ArgumentationFramework::isCredulousStableEarlyStop(const std::string& argument, int& counter) const {
    bool found = false;
    enumerateStableExtensionsWithEarlyStop(argument, counter, found);
    return found;
}


bool ArgumentationFramework::isSkepticalStable(const std::string& argument, int& counter) const {
    counter = 0;
    auto extensions = enumerateStableExtensions(counter);
    for (const auto& ext : extensions) {
        if (ext.find(argument) == ext.end()) return false;
    }
    return true;
}

// ------------------------------------
// APPROCHE FONCTION CARACTÉRISTIQUE
// ------------------------------------

std::set<std::string> ArgumentationFramework::characteristicFunction(const std::set<std::string>& S) const {
    std::set<std::string> defendedArguments;

    for (const auto& arg : arguments) {
        bool defended = true;

        for (size_t i = 0; i < arguments.size(); ++i) {
            if (attackMatrix[i][argumentIndices.at(arg)]) {
                bool defendedByS = false;

                for (const auto& defender : S) {
                    if (attackMatrix[argumentIndices.at(defender)][i]) {
                        defendedByS = true;
                        break;
                    }
                }

                if (!defendedByS) {
                    defended = false;
                    break;
                }
            }
        }

        if (defended) {
            defendedArguments.insert(arg);
        }
    }

    return defendedArguments;
}

// Crédulous Complete (Fonction caractéristique)
bool ArgumentationFramework::isCredulousCompletePlus(const std::string& argument, int& counter) const {
    std::vector<std::set<std::string>> visitedExtensions;

    std::function<bool(std::set<std::string>)> exploreComplete = [&](std::set<std::string> current) {
        counter++; 
        auto next = characteristicFunction(current); 

        if (next == current) { 
            if (std::find(visitedExtensions.begin(), visitedExtensions.end(), next) == visitedExtensions.end()) {
                visitedExtensions.push_back(next);
                if (next.find(argument) != next.end()) {
                    return true;
                }
            }
            return false;
        }

        return exploreComplete(next);
    };

    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg};
        if (exploreComplete(startSet)) {
            return true;
        }
    }

    return false;
}


// Skeptical Complete (Fonction caractéristique)
bool ArgumentationFramework::isSkepticalCompletePlus(const std::string& argument, int& counter) const {
    counter = 0; 
    std::set<std::string> visitedExtensions; 
    std::vector<std::set<std::string>> completeExtensions;

    // Fonction récursive pour explorer toutes les extensions complètes
    std::function<void(std::set<std::string>)> exploreComplete = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // Applique la fonction caractéristique

        // Vérifie si on a trouvé un point fixe
        if (next == current) {
            std::string formatted = formatExtension(next); // Formate l'extension
            if (visitedExtensions.find(formatted) == visitedExtensions.end()) {
                visitedExtensions.insert(formatted); // Ajoute au set des extensions visitées

                // Vérifie si l'argument est absent de cette extension
                if (next.find(argument) == next.end()) {
                    completeExtensions.push_back(next); // Ajoute l'extension à la liste
                    throw std::runtime_error("Argument absent d'une extension"); // stop l'exploration
                }

                completeExtensions.push_back(next);  // Ajoute l'extension complète à la liste
            }
            return;
        }

        // Continue l'exploration uniquement si l'ensemble actuel n'a pas encore été visité
        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent);
            exploreComplete(next); // Explore récursivement
        }
    };

    // Démarre l'exploration avec tous les ensembles possibles
    try {
        for (const auto& arg : arguments) {
            std::set<std::string> startSet = {arg};
            exploreComplete(startSet);
        }
    } catch (const std::runtime_error&) {
        return false; // Dès qu'une extension n'inclut pas l'argument, retourne "non sceptique"
    }

    return true; // Si toutes les extensions incluent l'argument, il est sceptiquement acceptable
}

// Crédulous Stable (Fonction caractéristique)

bool ArgumentationFramework::isCredulousStablePlus(const std::string& argument, int& counter) const {
    counter = 0; 
    std::vector<std::set<std::string>> visitedExtensions;

    std::function<bool(std::set<std::string>)> exploreStable = [&](std::set<std::string> current) {
        counter++; // Incrémente chaque état exploré
        auto next = characteristicFunction(current); // Applique la fonction caractéristique

        // Vérifie si on a trouvé un point fixe (extension stable)
        if (next == current) {
            if (std::find(visitedExtensions.begin(), visitedExtensions.end(), next) == visitedExtensions.end()) {
                visitedExtensions.push_back(next);
                if (isStable(next) && next.find(argument) != next.end()) {
                    return true; // L'argument est dans une extension stable
                }
            }
            return false; // Cette extension ne contient pas l'argument
        }

        return exploreStable(next); // Continue jusqu'à trouver un point fixe
    };

    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg};
        if (exploreStable(startSet)) {
            return true; // L'argument est trouvé dans une extension stable
        }
    }

    return false; // Aucun chemin trouvé contenant l'argument
}
// Skeptical Stable (Fonction caractéristique)
bool ArgumentationFramework::isSkepticalStablePlus(const std::string& argument, int& counter) const {
    counter = 0; 
    std::set<std::string> visitedExtensions; // Stocke les extensions explorées
    std::vector<std::set<std::string>> stableExtensions; // Stocke les extensions stables

    // Fonction récursive pour explorer toutes les extensions stables
    std::function<void(std::set<std::string>)> exploreStable = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // Applique la fonction caractéristique

        // Vérifie si on a trouvé un point fixe et que l'extension est stable
        if (next == current && isStable(next)) {
            std::string formatted = formatExtension(next); // Formate l'extension
            if (visitedExtensions.find(formatted) == visitedExtensions.end()) {
                visitedExtensions.insert(formatted); // Ajoute au set des extensions visitées

                // Vérifie si l'argument est absent de cette extension
                if (next.find(argument) == next.end()) {
                    stableExtensions.push_back(next); // Ajoute l'extension à la liste
                    throw std::runtime_error("Argument absent d'une extension"); // Stoppe l'exploration
                }

                stableExtensions.push_back(next);  // Ajoute l'extension stable à la liste
            }
            return;
        }

        // Continue l'exploration uniquement si l'ensemble actuel n'a pas encore été visité
        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent);
            exploreStable(next); // Explore récursivement
        }
    };

    // Démarre l'exploration avec tous les ensembles possibles
    try {
        for (const auto& arg : arguments) {
            std::set<std::string> startSet = {arg};
            exploreStable(startSet);
        }
    } catch (const std::runtime_error&) {
        return false; // Dès qu'une extension stable n'inclut pas l'argument, retourne "non sceptique"
    }

    return true; // Si toutes les extensions stables incluent l'argument, il est sceptiquement acceptable
}


std::set<std::string> ArgumentationFramework::findCompleteExtensionPlus(int& counter) const {
    counter = 0; 

    std::set<std::string> visitedExtensions; // Garde la trace des extensions visitées

    std::function<std::set<std::string>(std::set<std::string>)> exploreComplete = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // Applique la fonction caractéristique

        if (next == current) { // Point fixe trouvé
            return next; // Retourne l'extension complète
        }

        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent); // Marque l'extension comme visitée
            return exploreComplete(next); // Continue l'exploration
        }

        return std::set<std::string>{}; // Si déjà visité, retourne un ensemble vide
    };

    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg};
        auto result = exploreComplete(startSet);
        if (!result.empty()) {
            return result; // Retourne la première extension complète trouvée
        }
    }

    return {}; // Retourne un ensemble vide si aucune extension n'est trouvée (impossible normalement)
}

std::set<std::string> ArgumentationFramework::findStableExtensionPlus(int& counter) const {
    counter = 0; 
    std::set<std::string> visitedExtensions; // Garde la trace des extensions visitées

    std::function<std::set<std::string>(std::set<std::string>)> exploreStable = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // Applique la fonction caractéristique

        if (next == current && isStable(next)) { // Point fixe trouvé et extension stable
            return next;
        }

        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent); // Marque l'extension comme visitée
            return exploreStable(next); // Continue l'exploration
        }

        return std::set<std::string>{}; // Retourne un ensemble vide si déjà visité
    };

    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg};
        auto result = exploreStable(startSet);
        if (!result.empty()) {
            return result; // Retourne la première extension stable trouvée
        }
    }

    return {}; // Retourne un ensemble vide si aucune extension stable n'est trouvée
}








