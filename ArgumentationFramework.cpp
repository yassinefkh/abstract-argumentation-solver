#include "ArgumentationFramework.h"

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
    std::cout << ">>> Arguments :\n";
    for (const auto& arg : arguments) {
        std::cout << arg << "\n";
    }

    std::cout << "\n>>> Attacks:\n";
    for (const auto& attack : attacks) {
        std::cout  << attack.first << " -> " << attack.second << "\n";
    }
}

bool ArgumentationFramework::isConflictFree(const std::set<std::string>& extension) const {
    for (const auto& arg : extension) {
        if (arguments.find(arg) == arguments.end()) {
            std::cerr << "Error: Argument '" << arg << "' is not in the AF.\n";
            return false;
        }
    }
    for (const auto& arg1 : extension) {
        for (const auto& arg2 : extension) {
            std::cout << "Checking attack: " << arg1 << " -> " << arg2 << "\n";
            if (std::find(attacks.begin(), attacks.end(), std::make_pair(arg1, arg2)) != attacks.end()) {
                std::cout << "Conflict found: " << arg1 << " -> " << arg2 << "\n";
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
                std::cout << "Argument '" << arg << "' is NOT defended by the extension.\n";
                return false;
            }
        }
    }
    std::cout << "Argument '" << arg << "' is defended by the extension.\n";
    return true;
}


bool ArgumentationFramework::isAdmissible(const std::set<std::string>& extension) const {
    return isConflictFree(extension) && std::all_of(extension.begin(), extension.end(),
        [this, &extension](const std::string& arg) { return defends(arg, extension); });
}

bool ArgumentationFramework::isComplete(const std::set<std::string>& extension) const {
    if (!isAdmissible(extension)) {
         std::cout << "not admissible" << std::endl;
         return false;
    }
  
    for (const auto& arg : arguments) {
        if (defends(arg, extension)) {
            if (extension.find(arg) == extension.end()) {
                std::cout << "Argument '" << arg << "' is defended but not in the extension.\n";
                return false;
            }
        }
    }
    return true;

}