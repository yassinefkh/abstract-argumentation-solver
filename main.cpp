#include "Parser.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [input_file]\n";
        return 1;
    }

    try {
        Parser parser(argv[1]);

        ArgumentationFramework af = parser.parse();

        af.display();
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
        return 1;
    }

    return 0;
}
