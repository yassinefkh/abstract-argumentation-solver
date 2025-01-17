#include "utility.h"

// fonction pour formater un ensemble d'arguments sous forme de chaine de caracteres
std::string formatExtension(const std::set<std::string>& extension) {
    std::string result = "[";
    for (auto it = extension.begin(); it != extension.end(); ++it) {
        result += *it;
        if (std::next(it) != extension.end()) {
            result += ",";
        }
    }
    result += "]";
    return result;
}