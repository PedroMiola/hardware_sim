#include "../include/memory.hpp"
#include "../include/dataValue.hpp"
#include "../include/testUtils.hpp"

#include <deque>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

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
        logExpect(!dv.isValid(), failures, log, "Empty memory read() must return invalid");
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

        std::vector<DataValue<DATA_TYPE>> got;
        int n = drainAll(mem, got);
        logExpect(n == 0, failures, log, "Before A-th simulate(), memory should not have reacted");

        mem.simulate();
        n = drainAll(mem, got);
        logExpect(n == 3, failures, log, "On A-th simulate(), memory must read all available valid values");
        logExpect((got.size() >= 3) && (got[0].getValue() == 1.1) && (got[1].getValue() == 2.2) && (got[2].getValue() == 3.3) && got[0].isValid() && got[1].isValid() && got[2].isValid(),
               failures, log, "Read values must preserve source order");
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

        std::vector<DataValue<DATA_TYPE>> got;
        int n = drainAll(mem, got);
        bool ok_order = (n == 3) && (got[0].getValue() == 30.0) && (got[1].getValue() == 40.0 ) && (got[2].getValue() == 50.0) && got[0].isValid() && got[1].isValid() && got[2].isValid();
        logExpect(ok_order, failures, log, "Circular buffer keeps most-recent values; returns oldest-first among stored");
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
        logExpect(a.isValid() && a.getValue() == 7.0, failures, log, "First value read is 7.0");
        logExpect(b.isValid() && b.getValue() == 8.0, failures, log, "Second value read is 8.0");
        logExpect(!c.isValid(), failures, log, "Reading once buffer is empty returns invalid");
    }

    // ---------- Test 4: setSize / setAccessTime getters ----------
    {
        Memory mem;
        FakeSource src("Src4");
        mem.setSourceComponent(&src);
        mem.setSize(9);
        mem.setAccessTime(4);
        logExpect(mem.getSize() == 9, failures, log, "getSize() reflects setSize()");
        logExpect(mem.getAccessTime() == 4, failures, log, "getAccessTime() reflects setAccessTime()");
    }

    if (failures == 0) log << "All Memory tests PASSED." << std::endl;
    else log << failures << " Memory test(s) FAILED." << std::endl;
    log.close();
    return failures;
}
