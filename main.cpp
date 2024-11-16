#include "Parser.h"
#include <iostream>
#include <set>
#include <string>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::string filename = argv[1];

    try {
      
        Parser parser(filename);
        ArgumentationFramework af = parser.parse();
        af.display();

        std::set<std::string> conflictFreeTest =  {"a1"};
        std::set<std::string> admissibleTest =  {"a1"};
        std::set<std::string> completeTest = {"a1","a4","a6"};

        std::cout << "\nTesting conflict-free property for extension {a1}\n";
        if (af.isConflictFree(conflictFreeTest)) {
            std::cout << "The extension is conflict-free.\n";
        } else {
            std::cout << "The extension is not conflict-free.\n";
        }

        std::cout << "\nTesting admissibility for extension {a1}:\n";
        if (af.isAdmissible(admissibleTest)) {
            std::cout << "The extension is admissible.\n";
        } else {
            std::cout << "The extension is not admissible.\n";
        }


        std::cout << "\nTesting completeness for extension {a1}:\n";
        if (af.isComplete(completeTest)) {
            std::cout << "The extension is complete.\n";
        } else {
            std::cout << "The extension is not complete.\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
