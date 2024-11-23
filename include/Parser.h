#ifndef PARSER_H
#define PARSER_H

#include "ArgumentationFramework.h"
#include <string>
#include <stdexcept>

class Parser {
public:
    Parser(const std::string& filename);
    ArgumentationFramework parse();

private:
    std::string filename; 
};

#endif 
