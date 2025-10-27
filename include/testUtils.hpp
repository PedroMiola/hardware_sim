#include "../include/component.hpp"
#include "../include/dataValue.hpp"
#include <deque>
#include <vector>
#include <fstream>
#include <string>

class FakeSource : public ReadableComponent {
public:
    FakeSource(const std::string& lbl) { setType("FAKE_SOURCE"); setLabel(lbl); }

    void push(DATA_TYPE v) { queue.emplace_back(DataValue<DATA_TYPE>(v)); }
    void pushInvalid() { queue.emplace_back(DataValue<DATA_TYPE>()); }

    inline DataValue<DATA_TYPE> read() override {
        if (queue.empty()) return DataValue<DATA_TYPE>();
        DataValue<DATA_TYPE> dv = queue.front();
        queue.pop_front();
        return dv;
    }

    void simulate() override {}
    void load() override {}

    bool empty() const { return queue.empty(); }
    unsigned int size() const { return queue.size(); }

private:
    std::deque<DataValue<DATA_TYPE>> queue;
};

void logExpect(bool cond, int& fails, std::ofstream& log, const std::string& msg) {
    if (cond) { log << "[ OK ] " << msg << std::endl; }
    else { log << "[FAIL] " << msg << std::endl; ++fails; }
}

template <typename T>  
DataValue<DATA_TYPE> drainOne(T& t) {
    return t.read();
}

template <typename T> 
int drainAll(T& t, std::vector<DataValue<DATA_TYPE>>& out) {
    int count = 0;
    while (true) {
        auto dv = t.read();
        if (!dv.isValid()) break;
        out.push_back(dv);
        count++;
    }
    return count;
}
