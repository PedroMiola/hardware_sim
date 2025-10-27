#include "../include/memory.hpp"
#include "../include/dataValue.hpp"
#include <deque>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
class FakeSource : public ReadableComponent {
public:
    FakeSource(const std::string& label) { setType("FAKE_SOURCE"); setLabel(label); }

    void push(DATA_TYPE v) { queue.emplace_back(DataValue<DATA_TYPE>(v)); }
    void pushInvalid() { queue.emplace_back(DataValue<DATA_TYPE>()); }

    DataValue<DATA_TYPE> read() override {
        if (queue.empty()) return DataValue<DATA_TYPE>();
        DataValue<DATA_TYPE> dv = queue.front();
        queue.pop_front();
        return dv;
    }

    void simulate() override {}
    void load() override {}

    bool empty() const { return queue.empty(); }
    int size() const { return queue.size(); }

private:
    std::deque<DataValue<DATA_TYPE>> queue;
};

static int drainMemory(Memory& mem, std::vector<DATA_TYPE>& out) {
    int count = 0;
    while (true) {
        auto dv = mem.read();
        if (!dv.isValid()) break;
        out.push_back(dv.getValue());
        ++count;
    }
    return count;
}

static void expect(bool cond, int& failures, std::ofstream& log, const std::string& msg) {
    if (!cond) {
        ++failures;
        log << "[FAIL] " << msg << std::endl;
    } else {
        log << "[ OK ] " << msg << std::endl;
    }
}

int main() {
    std::ofstream log("../log/outputMemory.log");
    if (!log) {
        std::cerr << "Error: Unable to open ../log/outputMemory.log" << std::endl;
        return 1;
    }

    int failures = 0;

    // ---------- Test 0: reading from an empty memory returns invalid ----------
    {
        Memory mem;
        FakeSource src("Src0");
        mem.setSourceComponent(&src);
        mem.setSize(4);
        mem.setAccessTime(2);

        auto dv = mem.read();
        expect(!dv.isValid(), failures, log, "Empty memory read() must return invalid");
    }

    // ---------- Test 1: reacts only one over A times; reads ALL valid values when it reacts ----------
    {
        Memory mem;
        FakeSource src("Src1");
        mem.setSourceComponent(&src);
        mem.setSize(8);
        mem.setAccessTime(3); 

        src.push(1.1); src.push(2.2); src.push(3.3);

        mem.simulate();
        mem.simulate();

        std::vector<DATA_TYPE> got;
        int n = drainMemory(mem, got);
        expect(n == 0, failures, log, "Before A-th simulate(), memory should not have reacted");

        mem.simulate();
        n = drainMemory(mem, got);
        expect(n == 3, failures, log, "On A-th simulate(), memory must read all available valid values");
        expect((got.size() >= 3) && (got[0] == 1.1) && (got[1] == 2.2) && (got[2] == 3.3),
               failures, log, "Read values must preserve source order (1.1, 2.2, 3.3)");
    }

    // ---------- Test 2: circular buffer semantics (overwrite oldest when full) ----------
    {
        Memory mem;
        FakeSource src("Src2");
        mem.setSourceComponent(&src);
        mem.setSize(3);      
        mem.setAccessTime(1); 

        src.push(10.0); src.push(20.0); src.push(30.0); src.push(40.0); src.push(50.0);

        mem.simulate();

        std::vector<DATA_TYPE> got;
        int n = drainMemory(mem, got);
        bool ok_order = (n == 3) && got[0] == 30.0 && got[1] == 40.0 && got[2] == 50.0;
        expect(ok_order, failures, log, "Circular buffer keeps most-recent values; returns oldest-first among stored (30,40,50)");
    }

    // ---------- Test 3: after reading all stored values, further reads are invalid ----------
    {
        Memory mem;
        FakeSource src("Src3");
        mem.setSourceComponent(&src);
        mem.setSize(4);
        mem.setAccessTime(1);
        src.push(7.0); src.push(8.0);

        mem.simulate();

        auto a = mem.read();
        auto b = mem.read();
        auto c = mem.read(); // should be invalid
        expect(a.isValid() && a.getValue() == 7.0, failures, log, "First value read is 7.0");
        expect(b.isValid() && b.getValue() == 8.0, failures, log, "Second value read is 8.0");
        expect(!c.isValid(), failures, log, "Reading once buffer is empty returns invalid");
    }

    // ---------- Test 4: setSize / setAccessTime getters ----------
    {
        Memory mem;
        FakeSource src("Src4");
        mem.setSourceComponent(&src);
        mem.setSize(9);
        mem.setAccessTime(4);
        expect(mem.getSize() == 9, failures, log, "getSize() reflects setSize()");
        expect(mem.getAccessTime() == 4, failures, log, "getAccessTime() reflects setAccessTime()");
    }

    // Summary
    if (failures == 0) {
        log << "All Memory tests PASSED." << std::endl;
    } else {
        log << failures << " Memory test(s) FAILED." << std::endl;
    }
    log.close();
    return failures;
}
