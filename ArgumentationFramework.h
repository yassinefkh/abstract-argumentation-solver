#ifndef ARGUMENTATION_FRAMEWORK_H
#define ARGUMENTATION_FRAMEWORK_H

#include <set>
#include <vector>
#include <string>
#include <utility>
#include <iostream>

class ArgumentationFramework {
public:

    void addArgument(const std::string& arg);
    void addAttack(const std::string& attacker, const std::string& target);
    void display() const;

    const std::set<std::string>& getArguments() const;
    const std::vector<std::pair<std::string, std::string>>& getAttacks() const;

private:
    std::set<std::string> arguments;                    
    std::vector<std::pair<std::string, std::string>> attacks; 
};

#endif 
