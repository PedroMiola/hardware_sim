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

void Bus::load(){ //TODO: Make load
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