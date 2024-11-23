#include "Parser.h"
#include "utility.h"
#include "ArgumentationFramework.h"
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
        // parse the input file and build the argumentation framework
        Parser parser(filename);
        ArgumentationFramework af = parser.parse();
        af.display();

        // extensions to test
        std::set<std::string> conflictFreeTest = {"a"};
        std::set<std::string> admissibleTest = {"a"};
        std::set<std::string> completeTest = {"a", "b", "c"};
        std::set<std::string> stableTest = {"c", "b"};

        // test conflict-free 
        std::cout << "\nTesting conflict-free property for extension " 
                  << formatExtension(conflictFreeTest) << ":\n";
        if (af.isConflictFree(conflictFreeTest)) {
            std::cout << "The extension is conflict-free.\n";
        } else {
            std::cout << "The extension is not conflict-free.\n";
        }

        // test admissibility
        std::cout << "\nTesting admissibility for extension " 
                  << formatExtension(admissibleTest) << ":\n";
        if (af.isAdmissible(admissibleTest)) {
            std::cout << "The extension is admissible.\n";
        } else {
            std::cout << "The extension is not admissible.\n";
        }

        // test completeness
        std::cout << "\nTesting completeness for extension " 
                  << formatExtension(completeTest) << ":\n";
        if (af.isComplete(completeTest)) {
            std::cout << "The extension is complete.\n";
        } else {
            std::cout << "The extension is not complete.\n";
        }

        // test stability
        std::cout << "\nTesting stability for extension " 
                  << formatExtension(stableTest) << ":\n";
        if (af.isStable(stableTest)) {
            std::cout << "The extension is stable.\n";
        } else {
            std::cout << "The extension is not stable.\n";
        }

        // find and display a stable extension
        std::cout << "\nSearching for a stable extension:\n";
        std::set<std::string> stableExtension = af.findStableExtension();
        if (!stableExtension.empty()) {
            std::cout << "Found stable extension: " 
                      << formatExtension(stableExtension) << "\n";
        } else {
            std::cout << "No stable extension found.\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
