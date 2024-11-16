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
