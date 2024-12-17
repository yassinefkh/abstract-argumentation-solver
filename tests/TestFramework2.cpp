#include <iostream>
#include <cassert>
#include "../include/ArgumentationFramework.h"
#include "../include/utility.h" 

#include <sstream>
#include <set>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

ArgumentationFramework createSampleFramework() {
    ArgumentationFramework af;

    // Add arguments
    af.addArgument("a");
    af.addArgument("b");
    af.addArgument("c");
    af.addArgument("d");
    af.addArgument("e");

    // Add attacks
    af.addAttack("a", "b");
    af.addAttack("b", "a");
    af.addAttack("a", "c");
    af.addAttack("b", "c");
    af.addAttack("c", "d");
    af.addAttack("d", "e");
    af.addAttack("e", "d");

    return af;
}

void testIsConflictFree() {
    std::cout << "Running testIsConflictFree..." << std::endl;
    ArgumentationFramework af = createSampleFramework();

    extern bool debugMode;
    debugMode = false;

    std::set<std::string> conflictFreeExtension = { };
    std::set<std::string> conflictingExtension = {"a", "b"};

    assert(af.isConflictFree(conflictFreeExtension) &&
           "Error: Conflict-free extension is incorrectly marked as conflicting.");

    assert(!af.isConflictFree(conflictingExtension) &&
           "Error: Conflicting extension is incorrectly marked as conflict-free.");

    std::cout << "testIsConflictFree passed!" << std::endl;
}

void testFindCompleteExtension() {
    std::cout << "Running testFindCompleteExtension..." << std::endl;

    ArgumentationFramework af = createSampleFramework();

    extern bool debugMode;
    debugMode = false;

    std::vector<std::set<std::string>> expectedExtensions = {
        {},           
        {"e"},   
        {"b"},  
        {"b","d"},           
        {"b", "e"},   
        {"a"},
        {"a","d"},           
        {"a", "e"}     
    };
    std::vector<std::set<std::string>> computedExtensions = af.enumerateCompleteExtensions();

    for (const auto& expected : expectedExtensions) {
        assert(std::find(computedExtensions.begin(), computedExtensions.end(), expected) != computedExtensions.end() &&
               "Error: Expected complete extension was not found.");
    }

    for (const auto& computed : computedExtensions) {
        assert(std::find(expectedExtensions.begin(), expectedExtensions.end(), computed) != expectedExtensions.end() &&
               "Error: Unexpected complete extension was found.");
    }

    std::cout << "testFindCompleteExtension passed!" << std::endl;
}



void testFindStableExtension() {
    std::cout << "Running testFindStableExtension..." << std::endl;

    ArgumentationFramework af = createSampleFramework();


    extern bool debugMode;
    debugMode = false;

    std::vector<std::set<std::string>> expectedExtensions = {
        {"b", "d"}, 
        {"b", "e"},  
        {"a", "e"}, 
        {"a", "d"}
    };
    std::vector<std::set<std::string>> computedExtensions = af.enumerateStableExtensions();

    for (const auto& expected : expectedExtensions) {
        assert(std::find(computedExtensions.begin(), computedExtensions.end(), expected) != computedExtensions.end() &&
               "Error: Expected stable extension was not found.");
    }

    for (const auto& computed : computedExtensions) {
        assert(std::find(expectedExtensions.begin(), expectedExtensions.end(), computed) != expectedExtensions.end() &&
               "Error: Unexpected stable extension was found.");
    }
    assert(computedExtensions.size() == expectedExtensions.size() &&
           "Error: Unexpected number of stable extensions.");

    std::cout << "testFindStableExtension passed!" << std::endl;
}



void testIsAdmissible() {
    std::cout << "Running testIsAdmissible..." << std::endl;

    ArgumentationFramework af = createSampleFramework();

    extern bool debugMode;
    debugMode = false;

    std::set<std::string> admissibleExtension = {"a","d"};    // Devrait être admissible
    std::set<std::string> nonAdmissibleExtension = {"a", "c"}; // Devrait être non admissible

    assert(af.isAdmissible(admissibleExtension) &&
           "Error: Expected admissible extension was not recognized as admissible.");

    assert(!af.isAdmissible(nonAdmissibleExtension) &&
           "Error: Non-admissible extension was incorrectly recognized as admissible.");

    std::cout << "testIsAdmissible passed!" << std::endl;
}

void testIsComplete() {
    std::cout << "Running testIsComplete..." << std::endl;

    ArgumentationFramework af = createSampleFramework();

    extern bool debugMode;
    debugMode = false;

    std::set<std::string> completeExtension = {"a", "d"};  
    std::set<std::string> incompleteExtension = {"c"};     

    assert(af.isComplete(completeExtension) &&
           "Error: Expected complete extension was not recognized as complete.");

    assert(!af.isComplete(incompleteExtension) &&
           "Error: Incomplete extension was incorrectly recognized as complete.");

    std::cout << "testIsComplete passed!" << std::endl;
}


void testEnumerateCompleteExtensions() {
    std::cout << "Running testEnumerateCompleteExtensions..." << std::endl;
    ArgumentationFramework af = createSampleFramework();

    std::vector<std::set<std::string>> expectedExtensions = {
        {},           
        {"e"},   
        {"b"},  
        {"b","d"},           
        {"b", "e"},   
        {"a"},
        {"a","d"},           
        {"a", "e"} 
    };

    std::vector<std::set<std::string>> computedExtensions = af.enumerateCompleteExtensions();

    std::cout << "Computed complete extensions:" << std::endl;
    for (const auto& ext : computedExtensions) {
        std::cout << formatExtension(ext) << std::endl;
    }

    for (const auto& expected : expectedExtensions) {
        if (std::find(computedExtensions.begin(), computedExtensions.end(), expected) == computedExtensions.end()) {
            std::cerr << "Error: Expected complete extension "
                      << formatExtension(expected)
                      << " was not found." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    for (const auto& computed : computedExtensions) {
        if (std::find(expectedExtensions.begin(), expectedExtensions.end(), computed) == expectedExtensions.end()) {
            std::cerr << "Error: Unexpected complete extension "
                      << formatExtension(computed)
                      << " was found." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    std::cout << "testEnumerateCompleteExtensions passed!" << std::endl;
}

void testEnumerateStableExtensions() {
    std::cout << "Running testEnumerateStableExtensions..." << std::endl;

    ArgumentationFramework af = createSampleFramework();

    extern bool debugMode;
    debugMode = false;

    std::vector<std::set<std::string>> expectedExtensions = {
        {"b", "d"}, 
        {"b", "e"},  
        {"a", "e"}, 
        {"a", "d"}
    };

    std::vector<std::set<std::string>> computedExtensions = af.enumerateStableExtensions();
    for (const auto& expected : expectedExtensions) {
        assert(std::find(computedExtensions.begin(), computedExtensions.end(), expected) != computedExtensions.end() &&
               "Expected stable extension was not found.");
    }
    assert(computedExtensions.size() == expectedExtensions.size() &&
           "Unexpected number of stable extensions.");

    std::cout << "testEnumerateStableExtensions passed!" << std::endl;
}


int main() {
    std::cout << "Running tests for ArgumentationFramework..." << std::endl;

    // Run individual tests
    testIsConflictFree();
    testFindCompleteExtension();
    testFindStableExtension();
    testIsAdmissible();
    testIsComplete();
    testEnumerateStableExtensions();
    testEnumerateCompleteExtensions();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
