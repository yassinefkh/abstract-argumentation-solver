#include "ArgumentationFramework.h"

void ArgumentationFramework::addArgument(const std::string& arg) {
    arguments.insert(arg);
}

void ArgumentationFramework::addAttack(const std::string& attacker, const std::string& target) {
    attacks.emplace_back(attacker, target);
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

const std::set<std::string>& ArgumentationFramework::getArguments() const {
    return arguments;
}

const std::vector<std::pair<std::string, std::string>>& ArgumentationFramework::getAttacks() const {
    return attacks;
}
