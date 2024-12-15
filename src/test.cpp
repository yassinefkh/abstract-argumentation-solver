#include "ArgumentationFramework.h"
#include "Parser.h"
#include <iostream>
#include <set>
#include <string>

void testDefends(const ArgumentationFramework& af, const std::string& argument, const std::set<std::string>& extension) {
    std::cout << "Testing defends for argument: " << argument << "\n";
    std::cout << "Extension: {";
    for (const auto& extArg : extension) {
        std::cout << extArg << " ";
    }
    std::cout << "}\n";

    bool result = af.defends(argument, extension);
    if (result) {
        std::cout << "Result: The extension defends the argument " << argument << ".\n";
    } else {
        std::cout << "Result: The extension does NOT defend the argument " << argument << ".\n";
    }
}

/* int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <AF file> <argument>\n";
        return 1;
    }

    std::string filename = argv[1];
    std::string argument = argv[2];

    try {
        // Parse the input file and build the argumentation framework
        Parser parser(filename);
        ArgumentationFramework af = parser.parse();

        // Display the parsed AF
        af.display();

        // Example extension to test (customize as needed)
        std::set<std::string> extension = {"b"};

        // Test the defends function
        testDefends(af, argument, extension);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
 */

// g++ -std=c++11 -o test -Iinclude src/test.cpp src/ArgumentationFramework.cpp src/Parser.cpp
// ./test af/defendsTest.txt c