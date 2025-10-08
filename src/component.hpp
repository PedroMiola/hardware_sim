#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
#include "dataValue.hpp"

#define DATA_TYPE double

class Component {
public:
    std::string type;
    std::string label;

    virtual void simulate() = 0;
    virtual void load() = 0;
    virtual std::string getType() = 0;
    virtual std::string getLabel() = 0;
    virtual void setLabel(const std::string &newLabel) = 0;
    virtual void setType(const std::string &newType) = 0;
}; 

class Readable_Component : public Component {
public:
    virtual DataValue<DATA_TYPE> read() = 0;
};

#endif