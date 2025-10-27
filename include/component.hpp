#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
#include "dataValue.hpp"

#define DATA_TYPE double

class Component {
protected:
    std::string type;
    std::string label;
public:

    virtual void simulate() = 0;
    virtual void load() = 0;

    //Setter and Getters
    virtual std::string getType() {return type;};
    virtual std::string getLabel(){return label;};
    virtual void setLabel(const std::string &newLabel) {label = newLabel;};
    virtual void setType(const std::string &newType) {type = newType;};
}; 

class ReadableComponent : public Component {
public:
    virtual DataValue<DATA_TYPE> read() = 0;
};

#endif