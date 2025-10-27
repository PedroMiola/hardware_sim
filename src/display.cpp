#include "../include/display.hpp"

DataValue<DATA_TYPE> Display::readSource() {
    return sourceComponent->read();
}

void Display::simulate() {
    if(accessCounter == refreshRate - 1){
        accessCounter = 0;
        DataValue<DATA_TYPE> dv = readSource();
        while(dv.isValid()){
            *outputStream << dv.getValue() << std::endl;
            dv = readSource();
        }
    } else {
        accessCounter++;
    }
}

void Display::setOutputStream(std::ostream &os) {
    outputStream = &os;
}

void Display::load() { //TODO: Make load
}

unsigned int Display::getRefreshRate() {
    return refreshRate;
}

void Display::setRefreshRate(const unsigned int rate) {
    refreshRate = rate;
}

void Display::setSourceComponent(ReadableComponent *newSource) {
    sourceComponent = newSource;
}