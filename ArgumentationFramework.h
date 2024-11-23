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

    bool isConflictFree(const std::set<std::string>& extension) const;
    bool defends(const std::string& arg, const std::set<std::string>& extension) const;
    bool isAdmissible(const std::set<std::string>& extension) const;
    bool isComplete(const std::set<std::string>& extension) const;
    bool isStable(const std::set<std::string>& extension) const;
    

    std::set<std::string> findCompleteExtension() const;
    std::set<std::string> findStableExtension() const;
    std::vector<std::set<std::string>> enumerateCompleteExtensions() const;
    


    bool isCredulousComplete(const std::string& argument) const;
    bool isSkepticalComplete(const std::string& argument) const;
    bool isSkepticalCompleteBis(const std::string& argument) const;
    bool isSkepticalStable(const std::string& argument) const;
    bool isCredulousStable(const std::string& argument) const;
    
    void display() const;

    const std::set<std::string>& getArguments() const;
    const std::vector<std::pair<std::string, std::string>>& getAttacks() const;

private:
    std::set<std::string> arguments;                    
    std::vector<std::pair<std::string, std::string>> attacks; 
};

#endif 
