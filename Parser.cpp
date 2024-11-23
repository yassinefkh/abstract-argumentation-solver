#include "Parser.h"
#include "utility.h"
#include <fstream>
#include <algorithm>

Parser::Parser(const std::string& filename) : filename(filename) {}

ArgumentationFramework Parser::parse() {
    ArgumentationFramework af;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

        if (line.rfind("arg(", 0) == 0) { 
            std::string arg = line.substr(4, line.size() - 6);
#if DEBUG
            std::cout << "Parsed argument: [" << arg << "]\n";
#endif
            af.addArgument(arg);
        } else if (line.rfind("att(", 0) == 0) { 
            size_t commaPos = line.find(',', 4);
            std::string arg1 = line.substr(4, commaPos - 4);
            std::string arg2 = line.substr(commaPos + 1, line.size() - commaPos - 3);
#if DEBUG
            std::cout << "Parsed attack: (" << arg1 << " -> " << arg2 << ")\n";
#endif
            af.addAttack(arg1, arg2);
        }
    }

    file.close();
    return af;
}