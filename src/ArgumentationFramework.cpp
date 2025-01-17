#include "ArgumentationFramework.h"
#include "utility.h"
#include <cstdlib>
#include <ctime>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <numeric>
#include <random>


// ------------------------------------
// AJOUT D'ARGUMENTS ET ATTAQUES
// ------------------------------------

void ArgumentationFramework::addArgument(const std::string& arg) {
    // verifie si l'argument n'existe pas déjà dans le AF
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
    // parcourt chaque paire d'arguments dans l'extension
    for (const auto& arg1 : extension) {
        for (const auto& arg2 : extension) {
            // vérifie si arg1 attaque arg2 dans la matrice des attaques
            if (attackMatrix[argumentIndices.at(arg1)][argumentIndices.at(arg2)]) {
                return false; // si une attaque est trouvée, l'extension n'est pas sans conflit
            }
        }
    }
    // si aucune attaque n'est trouvée, retourne vrai : l'extension est sans conflit
    return true;
}

bool ArgumentationFramework::defends(const std::string& arg, const std::set<std::string>& extension) const {
     // pour chaque argument attaquant l'argument donné
    for (size_t i = 0; i < arguments.size(); ++i) {
         // vierifie si l'attaque est contrée par au moins un argument de l'extension
        if (attackMatrix[i][argumentIndices.at(arg)]) {
            bool defended = false;
            for (const auto& defender : extension) {
                if (attackMatrix[argumentIndices.at(defender)][i]) {
                    defended = true; // l'attaque est contrée (défendue)
                    break;
                }
            }
            if (!defended) return false; // si aucune défense n'est trouvée, retourner faux
        }
    }
     // si toutes les attaques sont défendues, retourner vrai
    return true;
}

bool ArgumentationFramework::isAdmissible(const std::set<std::string>& extension) const {
     // une extension est admissible si elle est sans conflit et si chaque argument de l'extension est défendu
    return isConflictFree(extension) && std::all_of(extension.begin(), extension.end(),  // vérifier que tous les arguments de l'extension sont défendus
        [this, &extension](const std::string& arg) { return defends(arg, extension); });
}

bool ArgumentationFramework::isComplete(const std::set<std::string>& extension) const {
    // verifie si l'extension est admissible, sinon elle ne peut pas être complète
    if (!isAdmissible(extension)) return false;
    // pour chaque argument de l'AF
    for (const auto& arg : arguments) {
        // si l'argument est défendu par l'extension mais n'y appartient pas, alors ce n'est pas une extension complète
        if (defends(arg, extension) && extension.find(arg) == extension.end()) {
            return false;
        }
    }
    // si toutes les conditions sont satisfaites, l'extension est complète
    return true;
}

bool ArgumentationFramework::isStable(const std::set<std::string>& extension) const {
    // vérifier si l'extension est sans conflit
    if (!isConflictFree(extension)) return false;
    for (size_t i = 0; i < arguments.size(); ++i) {
         // vérifier si l'argument n'appartient pas à l'extension
        if (extension.find(arguments[i]) == extension.end()) {
            bool attacked = false;
            // vérifier si l'argument est attaqué par un élément de l'extension
            for (const auto& attacker : extension) {
                if (attackMatrix[argumentIndices.at(attacker)][i]) {
                    attacked = true;
                    break;
                }
            }
            // si l'argument n'est pas attaqué, alors l'extension n'est pas stable
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

    std::function<void(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            if (isComplete(current)) {
                extensions.push_back(current);
            }
            return;
        }

        // Ajout de l'argument actuel uniquement si cela maintient l'absence de conflits
        current.insert(arguments[index]);
        if (isConflictFree(current)) { // elagage sur l'absence de conflits
            explore(index + 1, current);
        }
        current.erase(arguments[index]); // Retour arrière

        // Exploration sans ajouter l'argument actuel
        explore(index + 1, current);
    };

    explore(0, {});
    return extensions;
}



std::vector<std::set<std::string>> ArgumentationFramework::enumerateStableExtensions(int& counter) const {
    counter = 0;
    std::vector<std::set<std::string>> extensions;
    size_t n = arguments.size();

    // Fonction récursive pour explorer les extensions stables
    std::function<void(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            // Vérifie si le sous-ensemble est stable
            if (isStable(current)) {
                extensions.push_back(current);
            }
            return;
        }

        // Ajout de l'argument actuel
        current.insert(arguments[index]);
        if (isConflictFree(current)) { // Élagage si le sous-ensemble est conflictuel
            explore(index + 1, current);
        }
        current.erase(arguments[index]); // Retour arrière

        // Exploration sans ajouter l'argument actuel
        explore(index + 1, current);
    };

    explore(0, {});
    return extensions;
}



std::set<std::string> ArgumentationFramework::findOneCompleteExtension(int& counter) const {
    size_t n = arguments.size();
    counter = 0;

    std::set<std::string> result;

    std::function<bool(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            if (isComplete(current)) {
                result = current; // Stocke la première extension complète trouvée
                return true; // Arrête l'exploration
            }
            return false;
        }

        // Ajoute l'argument actuel et continue l'exploration
        current.insert(arguments[index]);
        if (isConflictFree(current) && explore(index + 1, current)) {
            return true;
        }
        current.erase(arguments[index]); // Retour arrière

        // Explore sans ajouter l'argument actuel
        return explore(index + 1, current);
    };

    explore(0, {});
    return result; // Retourne l'extension trouvée ou un ensemble vide si aucune n'est trouvée
}


std::set<std::string> ArgumentationFramework::findOneStableExtension(int& counter) const {
    size_t n = arguments.size();
    counter = 0;

    std::set<std::string> result;

    std::function<bool(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            if (isStable(current)) {
                result = current; // Stocke la première extension stable trouvée
                return true; // Arrête l'exploration
            }
            return false;
        }

        // Ajoute l'argument actuel et continue l'exploration
        current.insert(arguments[index]);
        if (isConflictFree(current) && explore(index + 1, current)) {
            return true;
        }
        current.erase(arguments[index]); // Retour arrière

        // Explore sans ajouter l'argument actuel
        return explore(index + 1, current);
    };

    explore(0, {});
    return result; // Retourne l'extension trouvée ou un ensemble vide si aucune n'est trouvée
}


// ------------------------------------
// APPROCHE NAIVE : CREDULOUS & SKEPTICAL
// ------------------------------------

bool ArgumentationFramework::isCredulousComplete(const std::string& argument, int& counter) const {
    size_t n = arguments.size();
    counter = 0;

    std::function<bool(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            if (isComplete(current) && current.find(argument) != current.end()) {
                return true; // L'argument est trouvé dans une extension complète
            }
            return false;
        }

        // Ajoute l'argument actuel et continue l'exploration
        current.insert(arguments[index]);
        if (isConflictFree(current) && explore(index + 1, current)) {
            return true;
        }
        current.erase(arguments[index]); // Retour arrière

        // Explore sans ajouter l'argument actuel
        return explore(index + 1, current);
    };

    return explore(0, {});
}


// Vérifie si un argument est sceptiquement acceptable dans toutes les extensions complètes
// Retourne true si l'argument est présent dans toutes les extensions complètes, sinon false
bool ArgumentationFramework::isSkepticalComplete(const std::string& argument, int& counter) const {
    size_t n = arguments.size();
    counter = 0;

    std::function<bool(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            if (isComplete(current) && current.find(argument) == current.end()) {
                return false; // L'argument est absent d'une extension complète
            }
            return true;
        }

        // Ajoute l'argument actuel et continue l'exploration
        current.insert(arguments[index]);
        if (isConflictFree(current) && !explore(index + 1, current)) {
            return false;
        }
        current.erase(arguments[index]); // Retour arrière

        // Explore sans ajouter l'argument actuel
        if (!explore(index + 1, current)) {
            return false;
        }

        return true;
    };

    return explore(0, {});
}

// Vérifie si un argument est crédulement acceptable dans au moins une extension stable
// Retourne true si l'argument est présent dans au moins une extension stable, sinon false
bool ArgumentationFramework::isCredulousStable(const std::string& argument, int& counter) const {
    size_t n = arguments.size();
    counter = 0;

    std::function<bool(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            if (isStable(current) && current.find(argument) != current.end()) {
                return true; // L'argument est trouvé dans une extension stable
            }
            return false;
        }

        // Ajoute l'argument actuel et continue l'exploration
        current.insert(arguments[index]);
        if (isConflictFree(current) && explore(index + 1, current)) {
            return true;
        }
        current.erase(arguments[index]); // Retour arrière

        // Explore sans ajouter l'argument actuel
        return explore(index + 1, current);
    };

    return explore(0, {});
}


// Vérifie si un argument est sceptiquement acceptable dans toutes les extensions stables
// Retourne true si l'argument est présent dans toutes les extensions stables, sinon false
bool ArgumentationFramework::isSkepticalStable(const std::string& argument, int& counter) const {
    size_t n = arguments.size();
    counter = 0;

    std::function<bool(size_t, std::set<std::string>)> explore;
    explore = [&](size_t index, std::set<std::string> current) {
        if (index == n) {
            counter++;
            if (isStable(current) && current.find(argument) == current.end()) {
                return false; // L'argument est absent d'une extension stable
            }
            return true;
        }

        // Ajoute l'argument actuel et continue l'exploration
        current.insert(arguments[index]);
        if (isConflictFree(current) && !explore(index + 1, current)) {
            return false;
        }
        current.erase(arguments[index]); // Retour arrière

        // Explore sans ajouter l'argument actuel
        if (!explore(index + 1, current)) {
            return false;
        }

        return true;
    };

    return explore(0, {});
}


// ------------------------------------
// APPROCHE FONCTION CARACTÉRISTIQUE
// ------------------------------------

// Applique la fonction caractéristique pour calculer les arguments défendus par un ensemble donné
// Retourne l'ensemble des arguments défendus par l'ensemble S
std::set<std::string> ArgumentationFramework::characteristicFunction(const std::set<std::string>& S) const {
    // initialise un ensemble pour stocker les arguments défendus
    std::set<std::string> defendedArguments;

    // parcourt chaque argument de l'AF
    for (const auto& arg : arguments) {
        bool defended = true; // initialise à vrai pour chaque argument

        // vérifie si l'argument est attaqué par un autre argument
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (attackMatrix[i][argumentIndices.at(arg)]) {
                bool defendedByS = false;

                // vérifie si l'argument est défendu par au moins un argument de l'ensemble S
                for (const auto& defender : S) {
                    if (attackMatrix[argumentIndices.at(defender)][i]) {
                        defendedByS = true; // trouve un défenseur
                        break;
                    }
                }
                // si aucun défenseur n'est trouvé, marque l'argument comme non défendu
                if (!defendedByS) {
                    defended = false;
                    break;
                }
            }
        }

        // ajoute l'argument à l'ensemble défendu s'il est défendu
        if (defended) {
            defendedArguments.insert(arg);
        }
    }
    // retourne l'ensemble des arguments défendus
    return defendedArguments;
}

// Vérifie si un argument est acceptablement crédule pour les extensions complètes
// Utilise la fonction caractéristique pour explorer les points fixes correspondant aux extensions complètes
// Retourne true si l'argument est présent dans au moins une extension complète, sinon false
bool ArgumentationFramework::isCredulousCompletePlus(const std::string& argument, int& counter) const {
    // stocke les extensions déjà visitées pour éviter les explorations redondantes
    std::vector<std::set<std::string>> visitedExtensions;

    // fonction récursive pour explorer les extensions complètes
    std::function<bool(std::set<std::string>)> exploreComplete = [&](std::set<std::string> current) {
        counter++;  // incrémente le compteur pour chaque exploration
        auto next = characteristicFunction(current);  // applique la fonction caractéristique

        // vérifie si on a trouvé un point fixe
        if (next == current) { 
            // si l'extension n'a pas encore été visitée, on l'ajoute aux extensions visitées
            if (std::find(visitedExtensions.begin(), visitedExtensions.end(), next) == visitedExtensions.end()) {
                visitedExtensions.push_back(next);
                // vérifie si l'argument est présent dans l'extension
                if (next.find(argument) != next.end()) {
                    return true;
                }
            }
            return false; // retourne faux si l'argument n'est pas dans l'extension
        }

        // continue l'exploration si l'ensemble actuel n'a pas encore été visité
        return exploreComplete(next);
    };
    // démarre l'exploration pour chaque argument dans le graphe
    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg}; // initialise avec un seul argument
        if (exploreComplete(startSet)) { 
            return true; // retourne true si l'argument est trouvé dans une extension complète
        }
    }

    return false; // retourne false si aucune extension complète ne contient l'argument
}


// Skeptical Complete (Fonction caractéristique)
bool ArgumentationFramework::isSkepticalCompletePlus(const std::string& argument, int& counter) const {
    counter = 0; 
    std::set<std::string> visitedExtensions; 
    std::vector<std::set<std::string>> completeExtensions;

    // fonction récursive pour explorer toutes les extensions complètes
    std::function<void(std::set<std::string>)> exploreComplete = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // applique la fonction caractéristique

        // vérifie si on a trouvé un point fixe
        if (next == current) {
            std::string formatted = formatExtension(next); // formate l'extension
            if (visitedExtensions.find(formatted) == visitedExtensions.end()) {
                visitedExtensions.insert(formatted); // ajoute au set des extensions visitées

                // verifie si l'argument est absent de cette extension
                if (next.find(argument) == next.end()) {
                    completeExtensions.push_back(next); // ajoute l'extension à la liste
                    throw std::runtime_error("Argument absent d'une extension"); // stop l'exploration
                }

                completeExtensions.push_back(next);  // ajoute l'extension complète à la liste
            }
            return;
        }

        // continuer l'exploration uniquement si l'ensemble actuel n'a pas encore été visité
        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent);
            exploreComplete(next); // explore récursivement
        }
    };

    // demarrer l'exploration avec tous les ensembles possibles
    try {
        for (const auto& arg : arguments) {
            std::set<std::string> startSet = {arg};
            exploreComplete(startSet);
        }
    } catch (const std::runtime_error&) {
        return false; // dès qu'une extension n'inclut pas l'argument, retourne "non sceptique"
    }

    return true; // si toutes les extensions incluent l'argument, il est sceptiquement acceptable
}

// Crédulous Stable (Fonction caractéristique)
bool ArgumentationFramework::isCredulousStablePlus(const std::string& argument, int& counter) const {
    counter = 0; 
    std::vector<std::set<std::string>> visitedExtensions;

    std::function<bool(std::set<std::string>)> exploreStable = [&](std::set<std::string> current) {
        counter++; // incremente chaque état exploré
        auto next = characteristicFunction(current); // applique la fonction caractéristique

        // veirife si on a trouvé un point fixe (extension stable)
        if (next == current) {
            if (std::find(visitedExtensions.begin(), visitedExtensions.end(), next) == visitedExtensions.end()) {
                visitedExtensions.push_back(next);
                if (isStable(next) && next.find(argument) != next.end()) {
                    return true; // L'argument est dans une extension stable
                }
            }
            return false; // cette extension ne contient pas l'argument
        }

        return exploreStable(next); // continue jusqu'à trouver un point fixe
    };

    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg};
        if (exploreStable(startSet)) {
            return true; // l'argument est trouvé dans une extension stable
        }
    }

    return false; // aucun chemin trouvé contenant l'argument
}

// Skeptical Stable (Fonction caractéristique)
bool ArgumentationFramework::isSkepticalStablePlus(const std::string& argument, int& counter) const {
    counter = 0;
    std::set<std::string> visitedExtensions; // stocke les extensions explorées
    std::vector<std::set<std::string>> stableExtensions; // stocke les extensions stables

    // fonction récursive pour explorer toutes les extensions stables
    std::function<void(std::set<std::string>)> exploreStable = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // applique la fonction caractéristique

        // vérifie si on a trouvé un point fixe et que l'extension est stable
        if (next == current && isStable(next)) {
            std::string formatted = formatExtension(next); // formate l'extension
            if (visitedExtensions.find(formatted) == visitedExtensions.end()) {
                visitedExtensions.insert(formatted); // ajoute au set des extensions visitées

                // vérifie si l'argument est absent de cette extension
                if (next.find(argument) == next.end()) {
                    stableExtensions.push_back(next); // ajoute l'extension à la liste
                    throw std::runtime_error("Argument absent d'une extension"); // stop l'exploration
                }

                stableExtensions.push_back(next);  // ajoute l'extension stable à la liste
            }
            return;
        }

        // continue l'exploration uniquement si l'ensemble actuel n'a pas encore été visité
        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent);
            exploreStable(next); // epxlore récursivement
        }
    };

    // demarre l'exploration avec tous les ensembles possibles
    try {
        for (const auto& arg : arguments) {
            std::set<std::string> startSet = {arg};
            exploreStable(startSet);
        }
    } catch (const std::runtime_error&) {
        return false; // dès qu'une extension stable n'inclut pas l'argument, retourne "non sceptique"
    }

    return true; // si toutes les extensions stables incluent l'argument, il est sceptiquement acceptable
}

// Trouve une extension complète à l'aide de la fonction caractéristique
// Parcourt les arguments pour explorer les ensembles atteignant un point fixe
// Retourne une extension complète si trouvée, sinon retourne un ensemble vide
std::set<std::string> ArgumentationFramework::findCompleteExtensionPlus(int& counter) const {
    counter = 0; 

    std::set<std::string> visitedExtensions; // garde la trace des extensions visitées

    std::function<std::set<std::string>(std::set<std::string>)> exploreComplete = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // applique la fonction caractéristique

        if (next == current) { // point fixe trouvé
            return next; // retourne l'extension complète
        }

        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent); // marque l'extension comme visitée
            return exploreComplete(next); // continue l'exploration
        }

        return std::set<std::string>{}; // si déjà visité, retourne un ensemble vide
    };

    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg};
        auto result = exploreComplete(startSet);
        if (!result.empty()) {
            return result; // retourne la première extension complète trouvée
        }
    }

    return {}; // retourne un ensemble vide si aucune extension n'est trouvée 
}

// Trouve une extension stable à l'aide de la fonction caractéristique
// Parcourt les arguments pour explorer les ensembles atteignant un point fixe stable
// Retourne une extension stable si trouvée, sinon retourne un ensemble vide
std::set<std::string> ArgumentationFramework::findStableExtensionPlus(int& counter) const {
    counter = 0; 
    std::set<std::string> visitedExtensions; // garde la trace des extensions visitées

    std::function<std::set<std::string>(std::set<std::string>)> exploreStable = [&](std::set<std::string> current) {
        counter++; 

        auto next = characteristicFunction(current); // applique la fonction caractéristique

        if (next == current && isStable(next)) { // point fixe trouvé et extension stable
            return next;
        }

        std::string formattedCurrent = formatExtension(current);
        if (visitedExtensions.find(formattedCurrent) == visitedExtensions.end()) {
            visitedExtensions.insert(formattedCurrent); // marque l'extension comme visitée
            return exploreStable(next); // continue l'exploration
        }

        return std::set<std::string>{}; // retourne un ensemble vide si déjà visité
    };

    for (const auto& arg : arguments) {
        std::set<std::string> startSet = {arg};
        auto result = exploreStable(startSet);
        if (!result.empty()) {
            return result; // retourne la première extension stable trouvée
        }
    }

    return {}; // retourne un ensemble vide si aucune extension stable n'est trouvée
}



// ------------------------------------
// APPROCHE LABELLING   
// ------------------------------------
std::unordered_map<std::string, std::string> ArgumentationFramework::labellingComplet(int& stateCounter) const {
    // initialisation des labels
    std::unordered_map<std::string, std::string> labels;
    for (const auto& arg : arguments) {
        labels[arg] = "undec";
    }

    bool updated = true;

    // maj des labels
    while (updated) {
        updated = false;

        for (const auto& arg : arguments) {
            if (labels[arg] == "undec") {
                bool allAttackersOut = true;
                bool hasAcceptedAttacker = false;

                for (size_t i = 0; i < arguments.size(); ++i) {
                    if (attackMatrix[i][argumentIndices.at(arg)]) {
                        if (labels[arguments[i]] == "undec") {
                            allAttackersOut = false;
                        } else if (labels[arguments[i]] == "in") {
                            hasAcceptedAttacker = true;
                            break;
                        }
                    }
                }

                if (allAttackersOut && !hasAcceptedAttacker) {
                    labels[arg] = "in";
                    updated = true;
                } else if (hasAcceptedAttacker) {
                    labels[arg] = "out";
                    updated = true;
                }
            }
        }
    }

    //  y a t il encore des arguments undec ?
    bool hasUndec = std::any_of(labels.begin(), labels.end(), [](const auto& entry) {
        return entry.second == "undec";
    });

    if (hasUndec) {
        // backtracking pour résoudre les arguments restants
        std::function<bool(size_t)> backtrack = [&](size_t idx) {
            stateCounter++; 

            if (idx == arguments.size()) {
                // vérfication de cohérence du labelling
                for (size_t i = 0; i < arguments.size(); ++i) {
                    const auto& arg = arguments[i];
                    if (labels[arg] == "in") {
                        for (size_t j = 0; j < arguments.size(); ++j) {
                            if (attackMatrix[j][i] && labels[arguments[j]] != "out") {
                                return false; // incohérence
                            }
                        }
                    } else if (labels[arg] == "out") {
                        bool defended = false;
                        for (size_t j = 0; j < arguments.size(); ++j) {
                            if (attackMatrix[j][i] && labels[arguments[j]] == "in") {
                                defended = true;
                                break;
                            }
                        }
                        if (!defended) return false; // incohérence
                    }
                }
                return true;
            }

            const auto& arg = arguments[idx];
            if (labels[arg] == "undec") {
                // essayer in
                labels[arg] = "in";
                if (backtrack(idx + 1)) return true;

                // essayer out
                labels[arg] = "out";
                if (backtrack(idx + 1)) return true;

                // reset
                labels[arg] = "undec";
            } else {
                return backtrack(idx + 1);
            }

            return false;
        };

        if (!backtrack(0)) {
            throw std::runtime_error("Backtracking failed.");
        }
    }

    return labels;
}


std::unordered_map<std::string, std::string> ArgumentationFramework::labellingStable(int& stateCounter) const {
    // initialisation des labels
    std::unordered_map<std::string, std::string> labels;
    for (const auto& arg : arguments) {
        labels[arg] = "undec";
    }

    bool updated = true;

    // maj des labels
    while (updated) {
        updated = false;

        for (const auto& arg : arguments) {
            if (labels[arg] == "undec") {
                bool allAttackersOut = true;
                bool hasAcceptedAttacker = false;

                for (size_t i = 0; i < arguments.size(); ++i) {
                    if (attackMatrix[i][argumentIndices.at(arg)]) {
                        if (labels[arguments[i]] == "undec") {
                            allAttackersOut = false;
                        } else if (labels[arguments[i]] == "in") {
                            hasAcceptedAttacker = true;
                            break;
                        }
                    }
                }

                if (allAttackersOut && !hasAcceptedAttacker) {
                    labels[arg] = "in";
                    updated = true;
                } else if (hasAcceptedAttacker) {
                    labels[arg] = "out";
                    updated = true;
                }
            }
        }
    }

    // y a t il encore des arguments undec ?
    bool hasUndec = std::any_of(labels.begin(), labels.end(), [](const auto& entry) {
        return entry.second == "undec";
    });

    if (hasUndec) {
        // backtracking pour résoudre les arguments restants
        std::function<bool(size_t)> backtrack = [&](size_t idx) {
            stateCounter++; // Incrémenter le compteur d'états

            if (idx == arguments.size()) {
                // verification de cohérence du labelling
                for (size_t i = 0; i < arguments.size(); ++i) {
                    const auto& arg = arguments[i];
                    if (labels[arg] == "in") {
                        for (size_t j = 0; j < arguments.size(); ++j) {
                            if (attackMatrix[j][i] && labels[arguments[j]] != "out") {
                                return false; // incohérence
                            }
                        }
                    } else if (labels[arg] == "out") {
                        // verification pour stabilité : chaque OUT doit être attaqué
                        bool attacked = false;
                        for (size_t j = 0; j < arguments.size(); ++j) {
                            if (attackMatrix[argumentIndices.at(arguments[j])][i] && labels[arguments[j]] == "in") {
                                attacked = true;
                                break;
                            }
                        }
                        if (!attacked) return false; // non stable si un OUT n'est pas attaqué
                    }
                }
                return true;
            }

            const auto& arg = arguments[idx];
            if (labels[arg] == "undec") {
                // essayer in
                labels[arg] = "in";
                if (backtrack(idx + 1)) return true;

                // essayer out
                labels[arg] = "out";
                if (backtrack(idx + 1)) return true;

                // réinitialiser
                labels[arg] = "undec";
            } else {
                return backtrack(idx + 1);
            }

            return false;
        };

        if (!backtrack(0)) {
            throw std::runtime_error("Backtracking failed.");
        }
    }

    return labels;
}


// Fonction pour afficher les résultats
void ArgumentationFramework::displayLabelling(const std::unordered_map<std::string, std::string>& labels) const {
    std::cout << "Labelling results:\n";
    for (const auto& [arg, label] : labels) {
        std::cout << "Argument " << arg << " is " << label << "\n";
    }
}