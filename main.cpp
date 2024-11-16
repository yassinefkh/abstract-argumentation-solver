#include "Parser.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Parser parser("af2.txt");
    ArgumentationFramework af = parser.parse();
    af.display();

    std::set<std::string> testExtension = {"a", "b"};
    std::cout << "Testing conflict-free property for extension {a1, a3}:\n";
    if (af.isConflictFree(testExtension)) {
        std::cout << "The extension is conflict-free.\n";
    } else {
        std::cout << "The extension is NOT conflict-free.\n";
    }

    return 0;
}
