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

void Display::load() {
    auto config = parseKeyValues();
    if(!config.count("REFRESH")) throw std::runtime_error("Display configuration file " + configFilePath + " missing REFRESH_RATE");
    if(!config.count("TYPE")) throw std::runtime_error("Display configuration file " + configFilePath + " missing TYPE");
    if(!config.count("SOURCE")) throw std::runtime_error("Display configuration file " + configFilePath + " missing SOURCE");
    if(config.count("LABEL") > 1) throw std::runtime_error("Display configuration file " + configFilePath + " has multiple LABEL entries");
    if(config.count("REFRESH") > 1) throw std::runtime_error("Display configuration file " + configFilePath + " has multiple REFRESH_RATE entries");
    if(config.count("TYPE") > 1) throw std::runtime_error("Display configuration file " + configFilePath + " has multiple TYPE entries");
    if(config.count("SOURCE") > 1) throw std::runtime_error("Display configuration file " + configFilePath + " has multiple SOURCE entries");

    if(config.size() > 4) throw std::runtime_error("Display configuration file " + configFilePath + " has unknown entries");

    if(!config.count("LABEL"))setLabel("Display 1");
    else setLabel(config["LABEL"][0]);
    setType(config["TYPE"][0]);
    setRefreshRate(std::stoi(config["REFRESH"][0]));
    setSourceComponent(nullptr);
    setSourceLabel(config["SOURCE"][0]);
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