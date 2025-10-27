#ifndef DATAVALUE_HPP
#define DATAVALUE_HPP

#include <utility>

template <typename T> class DataValue {
private:
    std::pair<T, bool> data;
public:
    DataValue() : data(std::make_pair(T(), false)) {}
    DataValue(T value) : data(std::make_pair(value, true)) {}

    T getValue() const {
        return data.first;
    }

    bool isValid() const {
        return data.second;
    }

    void setValue(T value) {
        data = std::make_pair(value, true);
    }

    void unset() {
        data.second = false;
    }
};

#endif //DATAVALUE_HPP