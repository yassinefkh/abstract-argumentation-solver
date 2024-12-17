#include "../include/ArgumentationFramework.h"
#include "../include/utility.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

// Helper for debug messages
bool debugMode = true;
void debug(const std::string& message) {
    if (debugMode) {
        std::cout << message << std::endl;
    }
}

// Add an argument
void ArgumentationFramework::addArgument(const std::string& arg) {
    arguments.insert(arg);
}

// Add an attack
void ArgumentationFramework::addAttack(const std::string& attacker, const std::string& target) {
    attacks.emplace_back(attacker, target);
}

// Get arguments
const std::set<std::string>& ArgumentationFramework::getArguments() const {
    return arguments;
}

// Get attacks
const std::vector<std::pair<std::string, std::string>>& ArgumentationFramework::getAttacks() const {
    return attacks;
}

// Display arguments and attacks
void ArgumentationFramework::display() const {
    std::cout << ">>> Arguments : [";
    for (auto it = arguments.begin(); it != arguments.end(); ++it) {
        std::cout << *it;
        if (std::next(it) != arguments.end()) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";

    std::cout << ">>> Attacks : [";
    for (size_t i = 0; i < attacks.size(); ++i) {
        std::cout << "(" << attacks[i].first << ", " << attacks[i].second << ")";
        if (i != attacks.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

//1
// Check if an extension is conflict-free
bool ArgumentationFramework::isConflictFree(const std::set<std::string>& extension) const {
    for (const auto& arg1 : extension) {
        for (const auto& arg2 : extension) {
            if (std::find(attacks.begin(), attacks.end(), std::make_pair(arg1, arg2)) != attacks.end()) {
                debug("Conflict found: " + arg1 + " -> " + arg2);
                return false;
            }
        }
    }
    return true;
}

// Check if an argument is defended by an extension
bool ArgumentationFramework::defends(const std::string& arg, const std::set<std::string>& extension) const {
    for (const auto& attack : attacks) {
        if (attack.second == arg) { // Check if `arg` is attacked
            bool defended = false;
            for (const auto& defender : extension) {
                if (std::find(attacks.begin(), attacks.end(), std::make_pair(defender, attack.first)) != attacks.end()) {
                    defended = true;
                    break;
                }
            }
            if (!defended) {
                debug("Argument '" + arg + "' is NOT defended by the extension.");
                return false;
            }
        }
    }
    debug("Argument '" + arg + "' is defended by the extension.");
    return true;
}

//2
// Check if an extension is admissible
bool ArgumentationFramework::isAdmissible(const std::set<std::string>& extension) const {
    return isConflictFree(extension) &&
           std::all_of(extension.begin(), extension.end(), [this, &extension](const std::string& arg) {
               return defends(arg, extension);
           });
}

//3
// Check if an extension is complete
bool ArgumentationFramework::isComplete(const std::set<std::string>& extension) const {
    if (!isAdmissible(extension)) {
        debug("Extension is not admissible.");
        return false;
    }
    for (const auto& arg : arguments) {
        if (defends(arg, extension) && extension.find(arg) == extension.end()) {
            debug("Argument '" + arg + "' is defended but not in the extension.");
            return false;
        }
    }
    return true;
}

//4
// Find one complete extension
std::set<std::string> ArgumentationFramework::findOneCompleteExtension() const {
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();
    for (size_t i = 0; i < (1U << n); ++i) { // Generate all subsets
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1U << j)) {
                subset.insert(args[j]);
            }
        }
        if (isComplete(subset)) {
            debug("Found complete extension: " + formatExtension(subset));
            return subset;
        }
    }
    return {};
}

//5
// Find one stable extension
std::set<std::string> ArgumentationFramework::findStableExtension() const {
    auto extensions = enumerateStableExtensions();
    if (!extensions.empty()) {
        return extensions[0]; // Return the first stable extension found
    }
    return {}; // Return an empty set if no stable extension exists
}

//6
// Enumerate all complete extensions
std::set<std::string> ArgumentationFramework::findCompleteExtension() const {
    auto extensions = enumerateCompleteExtensions();
    if (!extensions.empty()) {
        return extensions[0]; // Return the first complete extension found
    }
    return {}; // Return an empty set if no complete extension exists
}

//7
// Enumerate all stable extensions
std::vector<std::set<std::string>> ArgumentationFramework::enumerateCompleteExtensions() const {
    std::vector<std::set<std::string>> completeExtensions;
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();

    // Generate all subsets of arguments
    for (size_t i = 0; i < (1U << n); ++i) {
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1U << j)) {
                subset.insert(args[j]);
            }
        }

        // Check if the subset is a complete extension
        if (isComplete(subset)) {
            completeExtensions.push_back(subset);
        }
    }

    return completeExtensions;
}
//8
// Check if an extension is stable
bool ArgumentationFramework::isStable(const std::set<std::string>& extension) const {
    if (!isConflictFree(extension)) {
        return false;
    }
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

//9
// Enumerate all stable extensions
std::vector<std::set<std::string>> ArgumentationFramework::enumerateStableExtensions() const {
    std::vector<std::set<std::string>> stableExtensions;
    std::vector<std::string> args(arguments.begin(), arguments.end());
    size_t n = args.size();

    // Generate all subsets of arguments
    for (size_t i = 0; i < (1U << n); ++i) {
        std::set<std::string> subset;
        for (size_t j = 0; j < n; ++j) {
            if (i & (1U << j)) {
                subset.insert(args[j]);
            }
        }

        // Check if the subset is a stable extension
        if (isStable(subset)) {
            stableExtensions.push_back(subset);
        }
    }

    return stableExtensions;
}