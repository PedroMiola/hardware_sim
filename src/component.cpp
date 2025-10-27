#include "../include/component.hpp"

std::string trim(const std::string &s) {
    char *ws = " \t\r\n\f\v";
    unsigned int b = s.find_first_not_of(ws);
    if (b == std::string::npos) return std::string();
    unsigned int e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

std::unordered_map<std::string, std::vector<std::string>> Component::parseKeyValues(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Cannot open configuration file: " + filepath);
    }

    std::unordered_map<std::string, std::vector<std::string>> ouputKeys;
    std::string line;
    int lineNumber = 0;

    while(std::getline(file, line)){
        lineNumber++;

        std::string trimmedLine = trim(line);
        if(trimmedLine.empty() || trimmedLine[0] == '#') continue;

        unsigned int doubleDots = trimmedLine.find(':');

        if(doubleDots == std::string::npos){
            throw std::runtime_error("Syntax error in configuration file: " + filepath + " at line " + std::to_string(lineNumber) + ": missing ':'");
        }

        std::string key = trim(trimmedLine.substr(0, doubleDots));
        std::string value = trim(trimmedLine.substr(doubleDots + 1));

        std::transform(key.begin(), key.end(), key.begin(), [](auto c){ return std::toupper(c); });

        ouputKeys[key].push_back(value);
    }
    return ouputKeys;
}

std::vector<std::string> Component::splitWhitespace(const std::string &s) {
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    for (std::string tok; iss >> tok; ) tokens.push_back(tok);
    return tokens;
}