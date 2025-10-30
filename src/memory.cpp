#include "../include/memory.hpp"

DataValue<DATA_TYPE> Memory::readOldestData() {
    if (memory_buffer.empty()) {
        return DataValue<DATA_TYPE>();
    }
    DataValue<DATA_TYPE> oldestData = memory_buffer.front();
    memory_buffer.pop_front();
    return oldestData;
}

bool Memory::isFull() {
    return memory_buffer.size() >= size;
}

void Memory::storeData(const DataValue<DATA_TYPE> &data) {
    if (!isFull()) {
        memory_buffer.push_back(data);
    } else {
        memory_buffer.pop_front();
        memory_buffer.push_back(data);
    }
}

int Memory::getNumberOfStoredData() {
    return memory_buffer.size();
}

DataValue<DATA_TYPE> Memory::readSource() {
    return sourceComponent->read();
}

int Memory::getSize() {
    return size;
}

void Memory::setSize(int newSize) {
    size = newSize;
}

int Memory::getAccessTime() {
    return access_time;
}

void Memory::setAccessTime(int newAccessTime) {
    access_time = newAccessTime;
}

void Memory::setSourceComponent(ReadableComponent *newSource) {
    sourceComponent = newSource;
}

void Memory::simulate() {
    if(access_counter == access_time-1){
        access_counter = 0;
        DataValue<DATA_TYPE> newData = readSource();
        while(newData.isValid()){
            storeData(newData);
            newData = readSource();
        }
    } else {
        access_counter++;
    }
}
void Memory::load() { 
    auto config = parseKeyValues();

    if(!config.count("LABEL")) throw std::runtime_error("Memory configuration file " + configFilePath + " missing LABEL");
    if(!config.count("SIZE")) throw std::runtime_error("Memory configuration file " + configFilePath + " missing SIZE");
    if(!config.count("ACCESS")) throw std::runtime_error("Memory configuration file " + configFilePath + " missing ACCESS_TIME");
    if(!config.count("TYPE")) throw std::runtime_error("Memory configuration file " + configFilePath + " missing TYPE");
    if(!config.count("SOURCE")) throw std::runtime_error("Memory configuration file " + configFilePath + " missing SOURCE");
    if(config["LABEL"].size() > 1) throw std::runtime_error("Memory configuration file " + configFilePath + " has multiple LABEL entries");
    if(config["SIZE"].size() > 1) throw std::runtime_error("Memory configuration file " + configFilePath + " has multiple SIZE entries");
    if(config["ACCESS"].size() > 1) throw std::runtime_error("Memory configuration file " + configFilePath + " has multiple ACCESS_TIME entries");
    if(config["TYPE"].size() > 1) throw std::runtime_error("Memory configuration file " + configFilePath + " has multiple TYPE entries");
    if(config["SOURCE"].size() > 1) throw std::runtime_error("Memory configuration file " + configFilePath + " has multiple SOURCE entries");

    if(config.size() != 5) throw std::runtime_error("Memory configuration file " + configFilePath + " has unknown entries");

    setLabel(config["LABEL"][0]);
    setType(config["TYPE"][0]);
    setSize(std::stoi(config["SIZE"][0]));
    setAccessTime(std::stoi(config["ACCESS"][0]));
    setSourceComponent(nullptr);
    setSourceLabel(config["SOURCE"][0]);
}

DataValue<DATA_TYPE> Memory::read() {
    return readOldestData();
}