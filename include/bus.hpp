#ifndef BUS_HPP
#define BUS_HPP

#include "component.hpp"
#include "dataValue.hpp"
#include <queue>

class Bus : public ReadableComponent {
private:
    ReadableComponent* sourceComponent;
    unsigned int busWidth;
    unsigned int numberOfReads = 0;
    std::queue<DataValue<DATA_TYPE>> pendingDataValues;
    std::queue<DataValue<DATA_TYPE>> readyDataValues;
    std::string sourceCoponentLabel;

public:
    DataValue<DATA_TYPE> readSource();
    void moveValuesToReady();

    // Readable Components Methods
    void simulate() override;
    void load() override;
    DataValue<DATA_TYPE> read() override;

    // Setters and Getters
    const unsigned int getBusWidth();
    void setBusWidth(const unsigned int width);
    std::string getSourceLabel(){return sourceCoponentLabel;};
    void setSourceLabel(const std::string &newLabel) {sourceCoponentLabel = newLabel;};
    void setSourceComponent(ReadableComponent *newSource);

};

#endif