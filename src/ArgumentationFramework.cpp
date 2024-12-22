#include "ArgumentationFramework.h"
#include "utility.h"
#include <cstdlib>
#include <ctime>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <queue>

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
    counter = 0; // compteur pour suivre le nombre d'ensembles explorés
    std::vector<std::set<std::string>> extensions; // stocke les extensions complètes trouvées
    size_t n = arguments.size(); // nombre total d'arguments dans le système

    // parcourt tous les sous-ensembles possibles d'arguments (2^n combinaisons)
    for (size_t i = 0; i < (1 << n); ++i) {
        counter++;
        std::set<std::string> subset;

        // construit le sous-ensemble correspondant au bitmask actuel
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) subset.insert(arguments[j]); // ajoute l'argument si le bit est actif
        }
        // vérifie si le sous-ensemble est une extension complète
        if (isComplete(subset)) {
            extensions.push_back(subset); // ajoute l'extension complète à la liste des résultats
        }
    }
    return extensions; // on retourne la liste des extensions complètes
}

// de la même façon pour les extensions stables :
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
    return extensions; // retourne la liste des extensions stables
}

std::set<std::string> ArgumentationFramework::findOneCompleteExtension(int& counter) const {
    auto extensions = enumerateCompleteExtensions(counter);
    if (!extensions.empty()) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        return extensions[std::rand() % extensions.size()];
    }
    return {};
}

std::set<std::string> ArgumentationFramework::findOneStableExtension(int& counter) const {
    // génère toutes les extensions complètes et met à jour le compteur
    auto extensions = enumerateStableExtensions(counter);
    // vérifie si des extensions complètes ont été trouvées
    if (!extensions.empty()) {
        // initialise le générateur aléatoire avec le temps actuel pour garantir des résultats variés
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        // sélectionne une extension complète au hasard parmi les résultats trouvés
        return extensions[std::rand() % extensions.size()]; 
        // ici on fait ça pour vérifier qu'on a bien les extensions qu'on veut et pas plus, mais on pourrait
        // très bien se limiter qu'à UNE SEULE EXTENSION fixe trouvée !
    }
    return {};
}

// ------------------------------------
// APPROCHE NAIVE : CREDULOUS & SKEPTICAL
// ------------------------------------

bool ArgumentationFramework::isCredulousComplete(const std::string& argument, int& counter) const {
    counter = 0;
    // génère toutes les extensions complètes
    auto extensions = enumerateCompleteExtensions(counter);
    // parcourt chaque extension trouvée
    for (const auto& ext : extensions) {
        // si l'argument est présent dans une extension, retourne vrai
        if (ext.find(argument) != ext.end()) return true;
    }
    // retourne faux si l'argument n'est dans aucune extension complète
    return false;
}

// Vérifie si un argument est sceptiquement acceptable dans toutes les extensions complètes
// Retourne true si l'argument est présent dans toutes les extensions complètes, sinon false
bool ArgumentationFramework::isSkepticalComplete(const std::string& argument, int& counter) const {
    counter = 0;
    auto extensions = enumerateCompleteExtensions(counter);

    // parcourt chaque extension trouvée
    for (const auto& ext : extensions) {
        // si l'argument est absent d'une extension, retourne faux
        if (ext.find(argument) == ext.end()) return false;
    }
    // retourne true si l'argument est dans toutes les extensions complètes
    return true;
}

// Vérifie si un argument est crédulement acceptable dans au moins une extension stable
// Retourne true si l'argument est présent dans au moins une extension stable, sinon false
bool ArgumentationFramework::isCredulousStable(const std::string& argument, int& counter) const {
    counter = 0;
    auto extensions = enumerateStableExtensions(counter);
    // parcourt chaque extension trouvée
    for (const auto& ext : extensions) {
        // si l'argument est présent dans une extension, retourne vrai
        if (ext.find(argument) != ext.end()) return true;
    }
    // retourne faux si l'argument n'est présent dans aucune extension stable
    return false;
}

// Vérifie si un argument est sceptiquement acceptable dans toutes les extensions stables
// Retourne true si l'argument est présent dans toutes les extensions stables, sinon false
bool ArgumentationFramework::isSkepticalStable(const std::string& argument, int& counter) const {
    counter = 0;
    auto extensions = enumerateStableExtensions(counter);
    // parcourt chaque extension trouvée
    for (const auto& ext : extensions) {
          // si l'argument est absent d'une extension, retourne faux
        if (ext.find(argument) == ext.end()) return false;
    }
    // retourne vrai si l'argument est présent dans toutes les extensions stables
    return true;
}

// ------------------------------------
// APPROCHE FONCTION CARACTÉRISTIQUE
// ------------------------------------

// Applique la fonction caractéristique pour calculer les arguments défendus par un ensemble donné
// Retourne l'ensemble des arguments défendus par l'ensemble S
std::set<std::string> ArgumentationFramework::characteristicFunction(const std::set<std::string>& S) const {
    std::set<std::string> defendedArguments; // ensemble des arguments défendus
    std::set<std::string> visited; // ensemble pour suivre les arguments déjà visités

    auto isDefended = [&](const std::string& arg, const std::string& attacker) -> bool {
        // défense directe par un membre de S
        for (const auto& defender : S) {
            if (attackMatrix[argumentIndices.at(defender)][argumentIndices.at(attacker)]) {
                std::cout << "    " << arg << " defended by " << defender << " (directly)\n";
                return true;
            }
        }
        // defense transitive
        if (defendedArguments.find(attacker) != defendedArguments.end()) {
            std::cout << "    " << arg << " defended by " << attacker << " (indirectly via transitivity)\n";
            return true;
        }
        return false;
    };

    bool progress = true;
    while (progress) {
        progress = false;

        for (const auto& arg : arguments) { // pour chaque argument
            if (visited.find(arg) != visited.end()) continue; // ignorer les arguments déjà traités

            bool defended = true;
            for (size_t i = 0; i < arguments.size(); ++i) {
                if (attackMatrix[i][argumentIndices.at(arg)]) { // si l'argument est attaqué
                    if (!isDefended(arg, arguments[i])) {
                        defended = false;
                        break;
                    }
                }
            }

            if (defended) {
                defendedArguments.insert(arg); // ajouter aux arguments défendus
                visited.insert(arg); // marquer comme traité
                progress = true; // indiquer qu'un progrès a été réalisé
                std::cout << "  Added to defended arguments: " << arg << "\n";
            } else {
                std::cout << "  Not added to defended arguments: " << arg << "\n";
            }
        }
    }

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
    counter++;  
    auto next = characteristicFunction(current);  

    // Ajout de debug
    std::cout << "Exploring: ";
    for (const auto& arg : current) std::cout << arg << " ";
    std::cout << "\nNext: ";
    for (const auto& arg : next) std::cout << arg << " ";
    std::cout << "\n";

    if (next == current) {
        std::cout << "Point fixe atteint : ";
        for (const auto& arg : next) std::cout << arg << " ";
        std::cout << "\n";
        return next.find(argument) != next.end();
    }
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

// Vérifie si un graphe est acylique
bool ArgumentationFramework::isAcyclic() const {
    std::unordered_map<std::string, int> inDegree; // degré entrant des arguments
    std::queue<std::string> q;

    // initialisation des degrés entrants à 0
    for (const auto& arg : arguments) {
        inDegree[arg] = 0;
    }

    // calcul des degrés entrants à partir de la matrice des attaques
    for (size_t i = 0; i < arguments.size(); ++i) {
        for (size_t j = 0; j < arguments.size(); ++j) {
            if (attackMatrix[i][j]) {
                inDegree[arguments[j]]++;
            }
        }
    }

    // ajouter les arguments avec un degré entrant nul
    for (const auto& [arg, degree] : inDegree) {
        if (degree == 0) q.push(arg);
    }

    int visitedCount = 0; // nombre d'arguments visités
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        visitedCount++;

        // met à jour les degrés entrants des voisins
        for (size_t i = 0; i < arguments.size(); ++i) {
            if (arguments[i] == current) {
                for (size_t j = 0; j < arguments.size(); ++j) {
                    if (attackMatrix[i][j]) {
                        inDegree[arguments[j]]--;
                        if (inDegree[arguments[j]] == 0) {
                            q.push(arguments[j]);
                        }
                    }
                }
            }
        }
    }

    // si tous les arguments sont visités, le graphe est acyclique
    return static_cast<size_t>(visitedCount) == arguments.size();

}


// On calcule les étiquettes complètes pour chaque argument
std::unordered_map<std::string, std::string> ArgumentationFramework::computeCompleteLabelling() const {
    std::unordered_map<std::string, std::string> labels;

    // initialiser tous les arguments comme "undec"
    for (const auto& arg : arguments) {
        labels[arg] = "undec";
    }

    bool changed = true;
    while (changed) {
        changed = false;

        // parcourir chaque argument
        for (size_t i = 0; i < arguments.size(); ++i) {
            const std::string& arg = arguments[i];

            if (labels[arg] == "undec") {
                bool allAttackersOut = true;

                // parcourir les attaquants potentiels (colonne i dans attackMatrix)
                for (size_t j = 0; j < arguments.size(); ++j) {
                    if (attackMatrix[j][i]) { // j attaque i
                        if (labels[arguments[j]] == "in") {
                            labels[arg] = "out"; // argument attaqué par un "in"
                            changed = true;
                            break;
                        }
                        if (labels[arguments[j]] != "out") {
                            allAttackersOut = false;
                        }
                    }
                }

                // si tous les attaquants sont "out", marquer l'argument comme "in"
                if (allAttackersOut && labels[arg] == "undec") {
                    labels[arg] = "in";
                    changed = true;
                }
            }
        }
    }

    return labels;
}

// Calcul les étiquettes stables pour chaque argument
std::unordered_map<std::string, std::string> ArgumentationFramework::computeStableLabelling() const {
    auto labels = computeCompleteLabelling();

    // vérifie qu'il n'y a pas d'arguments étiquetés "undec"
    for (const auto& [arg, label] : labels) {
        if (label == "undec") {
            return {}; // retourne vide si le labelling complet n'est pas stable
        }
    }

    return labels;
}
