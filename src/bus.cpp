#include "../include/bus.hpp"

DataValue<DATA_TYPE> Bus::readSource() {
    return sourceComponent->read();
}

void Bus::moveValuesToReady() {
    while(!pendingDataValues.empty()) {
        readyDataValues.push(pendingDataValues.front());
        pendingDataValues.pop();
    }
}

void Bus::simulate() {
    moveValuesToReady();
    for(int i = 0; i < busWidth; i++) {
        DataValue<DATA_TYPE> newData = readSource();
        if(newData.isValid()) pendingDataValues.push(newData);
        else break;
    }
}

void Bus::load(){
    auto config = parseKeyValues();
    if(!config.count("LABEL")) throw std::runtime_error("Bus configuration file " + configFilePath + " missing LABEL");
    if(!config.count("WIDTH")) throw std::runtime_error("Bus configuration file " + configFilePath + " missing BUS_WIDTH");
    if(!config.count("TYPE")) throw std::runtime_error("Bus configuration file " + configFilePath + " missing TYPE");
    if(!config.count("SOURCE")) throw std::runtime_error("Bus configuration file " + configFilePath + " missing SOURCE");
    if(config["LABEL"].size() > 1) throw std::runtime_error("Bus configuration file " + configFilePath + " has multiple LABEL entries");
    if(config["WIDTH"].size() > 1) throw std::runtime_error("Bus configuration file " + configFilePath + " has multiple BUS_WIDTH entries");
    if(config["TYPE"].size() > 1) throw std::runtime_error("Bus configuration file " + configFilePath + " has multiple TYPE entries");
    if(config["SOURCE"].size() > 1) throw std::runtime_error("Bus configuration file " + configFilePath + " has multiple SOURCE entries");

    if(config.size() != 4) throw std::runtime_error("Bus configuration file " + configFilePath + " has unknown entries");

    setLabel(config["LABEL"][0]);
    setType(config["TYPE"][0]);
    setBusWidth(std::stoul(config["WIDTH"][0]));
    setSourceComponent(nullptr);
    setSourceLabel(config["SOURCE"][0]);
}

DataValue<DATA_TYPE> Bus::read() {
    numberOfReads++;
    if (readyDataValues.empty()) {
        return DataValue<DATA_TYPE>();
    } else {
        DataValue<DATA_TYPE> val = readyDataValues.front();
        readyDataValues.pop();
        return val;
    }
}

const unsigned int Bus::getBusWidth() {
    return busWidth;
}

void Bus::setBusWidth(const unsigned int width) {
    busWidth = width;
}

void Bus::setSourceComponent(ReadableComponent *newSource) {
    sourceComponent = newSource;
}