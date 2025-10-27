#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "component.hpp"
#include <deque>
#include <memory>

class Memory : public ReadableComponent {
private:
    std::deque<DataValue<DATA_TYPE>> memory_buffer;
    int size;
    int access_time;
    int access_counter;
    std::unique_ptr<ReadableComponent> sourceComponent;
public:
    DataValue<DATA_TYPE> readOldestData();
    DataValue<DATA_TYPE> readSource();
    bool isFull();
    void storeData(const DataValue<DATA_TYPE> &data);
    int getNumberOfStoredData();

    int getSize();
    void setSize(int newSize);
    int getAccessTime();
    void setAccessTime(int newAccessTime);
    std::string getSource();
    void setSource(const std::string &newSource);

    
    void simulate() override;
    void load() override;
    DataValue<DATA_TYPE> read() override;
};

#endif