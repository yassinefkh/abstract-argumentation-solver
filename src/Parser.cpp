#include "Parser.h"
#include "utility.h"
#include <fstream>
#include <algorithm>

Parser::Parser(const std::string& filename) : filename(filename) {}

// parse le fichier et construit l'AF
ArgumentationFramework Parser::parse() {
    ArgumentationFramework af;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // supprime tous les espaces blancs dans la ligne
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

        if (line.rfind("arg(", 0) == 0) { 
            // extrait le nom de l'argument (entre les parenthèses)
            std::string arg = line.substr(4, line.size() - 6);

            if (arg == "arg" || arg == "att" || 
                !std::all_of(arg.begin(), arg.end(), [](char c) { return std::isalnum(c) || c == '_'; })) {
                throw std::runtime_error("Error: Invalid argument name: " + arg);
            }

#if DEBUG
            std::cout << "Parsed argument: [" << arg << "]\n";
#endif
            af.addArgument(arg);

        } else if (line.rfind("att(", 0) == 0) { 
            // extrait les arguments de l'attaque 
            size_t commaPos = line.find(',', 4);
            std::string arg1 = line.substr(4, commaPos - 4);
            std::string arg2 = line.substr(commaPos + 1, line.size() - commaPos - 3);

            if (std::find(af.getArguments().begin(), af.getArguments().end(), arg1) == af.getArguments().end() || 
                std::find(af.getArguments().begin(), af.getArguments().end(), arg2) == af.getArguments().end()) {
                throw std::runtime_error("Error: Undefined arguments in attack: " + arg1 + ", " + arg2);
            }

#if DEBUG
            std::cout << "Parsed attack: (" << arg1 << " -> " << arg2 << ")\n";
#endif
            af.addAttack(arg1, arg2);

        } else {
            throw std::runtime_error("Error: Invalid line format: " + line);
        }
    }

    file.close();
    return af;
}
