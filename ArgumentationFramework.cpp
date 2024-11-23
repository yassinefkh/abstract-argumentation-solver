#include "ArgumentationFramework.h"
#include "utility.h"

void ArgumentationFramework::addArgument(const std::string& arg) {
    arguments.insert(arg);
}

void ArgumentationFramework::addAttack(const std::string& attacker, const std::string& target) {
    attacks.emplace_back(attacker, target);
}


const std::set<std::string>& ArgumentationFramework::getArguments() const {
    return arguments;
}

const std::vector<std::pair<std::string, std::string>>& ArgumentationFramework::getAttacks() const {
    return attacks;
}


void ArgumentationFramework::display() const {
#if DEBUG
    std::cout << ">>> Arguments :\n[";
    for (auto it = arguments.begin(); it != arguments.end(); ++it) {
        std::cout << *it;
        if (std::next(it) != arguments.end()) {
            std::cout << ",";
        }
    }
    std::cout << "]\n";

    std::cout << "\n>>> Attacks :\n[";
    for (size_t i = 0; i < attacks.size(); ++i) {
        std::cout << "(" << attacks[i].first << "," << attacks[i].second << ")";
        if (i != attacks.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << "]\n";
#endif
}



bool ArgumentationFramework::isConflictFree(const std::set<std::string>& extension) const {
    for (const auto& arg : extension) {
        if (arguments.find(arg) == arguments.end()) {
#if DEBUG
            std::cerr << "Error: Argument '" << arg << "' is not in the AF.\n";
#endif
            return false;
        }
    }
    for (const auto& arg1 : extension) {
        for (const auto& arg2 : extension) {
#if DEBUG
            std::cout << "Checking attack: " << arg1 << " -> " << arg2 << "\n";
#endif
            if (std::find(attacks.begin(), attacks.end(), std::make_pair(arg1, arg2)) != attacks.end()) {
#if DEBUG
                std::cout << "Conflict found: " << arg1 << " -> " << arg2 << "\n";
#endif
                return false;
            }
        }
    }
    return true;
}




bool ArgumentationFramework::defends(const std::string& arg, const std::set<std::string>& extension) const {
    for (const auto& attack : attacks) {
        if (attack.second == arg) {
            bool defended = false;
            for (const auto& defender : extension) {
                if (std::find(attacks.begin(), attacks.end(), std::make_pair(defender, attack.first)) != attacks.end()) {
                    defended = true;
                    break;
                }
            }
            if (!defended) {
#if DEBUG
                std::cout << "Argument '" << arg << "' is NOT defended by the extension.\n";
#endif
                return false;
            }
        }
    }
#if DEBUG
    std::cout << "Argument '" << arg << "' is defended by the extension.\n";
#endif
    return true;
}


bool ArgumentationFramework::isAdmissible(const std::set<std::string>& extension) const {
    return isConflictFree(extension) && std::all_of(extension.begin(), extension.end(),
        [this, &extension](const std::string& arg) { return defends(arg, extension); });
}

bool ArgumentationFramework::isComplete(const std::set<std::string>& extension) const {
    if (!isAdmissible(extension)) {
#if DEBUG
        std::cout << "not admissible\n";
#endif
        return false;
    }

    for (const auto& arg : arguments) {
        if (defends(arg, extension)) {
            if (extension.find(arg) == extension.end()) {
#if DEBUG
                std::cout << "Argument '" << arg << "' is defended but not in the extension.\n";
#endif
                return false;
            }
        }
    }
    return true;
}

// TODO :  a OTPIMISER ! ici on fait une recherche exhaustive, mais si on a un argument non attaqué on sait qu'il apartient à toute extension complète 
bool ArgumentationFramework::isCredulousComplete(const std::string& argument) const {
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();
    for (size_t i = 0; i < (1 << n); ++i) { 
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.insert(args[j]);
            }
        }
        if (isComplete(subset) && subset.find(argument) != subset.end()) {
            return true;
        }
    }
    return false;
}


bool ArgumentationFramework::isSkepticalCompleteBis(const std::string& argument) const {
    // si l'argument n'est pas attaqué, il est dans toutes les extensions complètes
    bool isAttacked = false;
    for (const auto& attack : attacks) {
        if (attack.second == argument) {
            isAttacked = true;
            break;
        }
    }
    if (!isAttacked) {
        return true;
    }

    // vérifier les défenses pour l'argument
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.insert(args[j]);
            }
        }

        if (isComplete(subset)) {
            // si l'argument est absent dans une extension complète, il n'est pas sceptiquement accepté.
            if (subset.find(argument) == subset.end()) {
                return false;
            }
        }
    }

    return true;
}


bool ArgumentationFramework::isStable(const std::set<std::string>& extension) const {
    if (!isConflictFree(extension)) return false;
    std::set<std::string> attacked;
    for (const auto& arg : extension) {
        for (const auto& attack : attacks) {
            if (attack.first == arg) {
                attacked.insert(attack.second);
            }
        }
    }
    for (const auto& arg : arguments) {
        if (extension.find(arg) == extension.end() && attacked.find(arg) == attacked.end()) {
            return false;
        }
    }
    return true;
}

// TODO : DOUBLONS ICI avec l'ensemble vide 
std::vector<std::set<std::string>> ArgumentationFramework::enumerateCompleteExtensions() const {
    std::vector<std::set<std::string>> completeExtensions;
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();

    // generate all subsets of arguments
    for (size_t i = 0; i < (1 << n); ++i) { 
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.insert(args[j]);
            }
        }

        // chick if the subset is a complete extension
        if (isComplete(subset)) {
            completeExtensions.push_back(subset);
        }
    }

    return completeExtensions;
}


bool ArgumentationFramework::isSkepticalComplete(const std::string& argument) const {
    std::vector<std::set<std::string>> completeExtensions = enumerateCompleteExtensions();

    for (const auto& extension : completeExtensions) {
        if (extension.find(argument) == extension.end()) {
            return false; 
        }
    }

    return true; 
}


std::set<std::string> ArgumentationFramework::findStableExtension() const {
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();
    for (size_t i = 0; i < (1 << n); ++i) {
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.insert(args[j]);
            }
        }
        if (isStable(subset)) {
#if DEBUG
            std::cout << "Found stable extension: " << formatExtension(subset) << "\n";
#endif
            return subset;
        }
    }
    return {};
}

bool ArgumentationFramework::isCredulousStable(const std::string& argument) const {
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.insert(args[j]);
            }
        }

        if (isStable(subset) && subset.find(argument) != subset.end()) {
            return true;
        }
    }

    return false;
}



bool ArgumentationFramework::isSkepticalStable(const std::string& argument) const {
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();

    for (size_t i = 0; i < (1 << n); ++i) {
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.insert(args[j]);
            }
        }

        if (isStable(subset) && subset.find(argument) == subset.end()) {
            return false;
        }
    }

    return true;
}
