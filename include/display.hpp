#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "component.hpp"
#include <iostream>
#include <ostream>

class Display : public Component {
private:
    ReadableComponent *sourceComponent;
    unsigned int refreshRate;
    unsigned int accessCounter = 0;
    std::ostream *outputStream = &std::cout;

public:
    DataValue<DATA_TYPE> readSource();
    void setOutputStream(std::ostream &os);

    // Component Methods
    void simulate() override;
    void load() override;

    // Setters and Getters
    unsigned int getRefreshRate();
    void setRefreshRate(const unsigned int rate);
    void setSourceComponent(ReadableComponent *newSource);
};


#endif