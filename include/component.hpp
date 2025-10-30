#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "dataValue.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

#define DATA_TYPE double

class Component {
protected:
    std::string type;
    std::string label;
    std::string configFilePath;
    std::string sourceCoponentLabel = "";


public:
    virtual std::unordered_map<std::string, std::vector<std::string>> parseKeyValues();
    virtual std::vector<std::string> splitWhitespace(const std::string &s);

    virtual void simulate() = 0;
    virtual void load() = 0;

    //Setter and Getters
    virtual std::string getType() {return type;};
    virtual std::string getLabel(){return label;};
    virtual std::string getConfigFilePath(){return configFilePath;};
    virtual void setLabel(const std::string &newLabel) {label = newLabel;};
    virtual void setType(const std::string &newType) {type = newType;};
    virtual void setConfigFilePath(const std::string &newPath) {configFilePath = newPath;};
    virtual std::string getSourceLabel(){return sourceCoponentLabel;};
    virtual void setSourceLabel(const std::string &newLabel) {sourceCoponentLabel = newLabel;};
}; 

class ReadableComponent : public Component {
public:
    virtual DataValue<DATA_TYPE> read() = 0;
};

#endif