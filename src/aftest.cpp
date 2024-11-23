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
            if (std::find(attacks.begin(), attacks.end(), std::make_pair(arg1, arg2)) != attacks.end()) {
                std::cout << "attaque trouvée" << std::endl;
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


std::set<std::string> ArgumentationFramework::findCompleteExtension() const {
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();
    for (size_t i = 0; i < (1 << n); ++i) { 
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.insert(args[j]);
            }
        }
        if (isComplete(subset)) return subset;
    }
    return {};
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
        if (isStable(subset)) return subset;
    }
    return {};
}

