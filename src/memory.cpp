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

std::string Memory::getSource() {
    return source;
}

void Memory::setSource(const std::string &newSource) {
    source = newSource;
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
void Memory::load() { //TODO implement loading from config file
}



DataValue<DATA_TYPE> Memory::read() {
    return readOldestData();
}