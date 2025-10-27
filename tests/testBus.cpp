#include "../include/bus.hpp"
#include "../include/testUtils.hpp"

#include <fstream>
#include <iostream>
#include <vector>

int main() {
    std::ofstream log("../log/outputBus.log");
    if (!log) {
        std::cerr << "Error: Unable to open ../log/outputBus.log" << std::endl;
        return 1;
    }

    int failures = 0;

    // ---------- Test 0: read() before any simulate() -> invalid ----------
    {
        Bus bus;
        FakeSource src("S0");
        bus.setSourceComponent(&src);
        bus.setBusWidth(2);

        DataValue<DATA_TYPE> dv = drainOne(bus);
        logExpect(!dv.isValid(), failures, log, "Initial read() returns invalid when no cycle has produced ready data");
    }

    // ---------- Test 1: one-cycle latency ----------
    // Values read from the source in cycle N become available in cycle N+1.
    {
        Bus bus;
        FakeSource src("S1");
        bus.setSourceComponent(&src);
        bus.setBusWidth(2);

        src.push(1.0);
        src.push(2.0);

        // Cycle 1: pending <- (1,2), ready empty
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            logExpect(n == 0, failures, log, "After first simulate(), no data is ready yet (1-cycle latency)");
        }

        // Cycle 2: ready <- (1,2)
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            bool okCount = (n == 2);
            logExpect(okCount, failures, log, "Second simulate() makes exactly 2 values available");
            if (okCount) {
                bool okVals = out[0].isValid() && out[1].isValid()
                              && out[0].getValue() == 1.0 && out[1].getValue() == 2.0;
                logExpect(okVals, failures, log, "Values preserved and ordered: 1.0, 2.0");
            }
        }
    }

    // ---------- Test 2: width chunking over multiple cycles ----------
    // With width=3 and 7 values, outputs should arrive as: (1,2,3), (4,5,6), (7)
    {
        Bus bus;
        FakeSource src("S2");
        bus.setSourceComponent(&src);
        bus.setBusWidth(3);

        for (int i = 1; i <= 7; ++i) src.push(static_cast<double>(i));

        // Cycle 1: pending <- (1,2,3), ready empty
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            logExpect(n == 0, failures, log, "No output on first cycle due to latency");
        }

        // Cycle 2: ready <- (1,2,3); pending <- (4,5,6)
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            bool okCount = (n == 3);
            logExpect(okCount, failures, log, "Cycle 2 outputs first width-chunk (size 3)");
            if (okCount) {
                bool okVals = out[0].getValue() == 1.0 && out[1].getValue() == 2.0 && out[2].getValue() == 3.0;
                logExpect(okVals, failures, log, "Cycle 2 values are 1, 2, 3 in order");
            }
        }

        // Cycle 3: ready <- (4,5,6); pending <- (7)
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            bool okCount = (n == 3);
            logExpect(okCount, failures, log, "Cycle 3 outputs second width-chunk (size 3)");
            if (okCount) {
                bool okVals = out[0].getValue() == 4.0 && out[1].getValue() == 5.0 && out[2].getValue() == 6.0;
                logExpect(okVals, failures, log, "Cycle 3 values are 4, 5, 6 in order");
            }
        }

        // Cycle 4: ready <- (7)
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            bool okCount = (n == 1);
            logExpect(okCount, failures, log, "Cycle 4 outputs the remaining element (size 1)");
            if (okCount) {
                bool okVal = out[0].getValue() == 7.0;
                logExpect(okVal, failures, log, "Cycle 4 value is 7");
            }
        }
    }

    // ---------- Test 3: fewer than width available ----------
    {
        Bus bus;
        FakeSource src("S3");
        bus.setSourceComponent(&src);
        bus.setBusWidth(4);

        src.push(10.0);
        src.push(20.0);

        // Cycle 1: pending <- (10,20); ready empty
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            logExpect(n == 0, failures, log, "First cycle still has no ready data");
        }

        // Cycle 2: ready <- (10,20)
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            bool okCount = (n == 2);
            logExpect(okCount, failures, log, "Second cycle outputs exactly the available 2 values");
            if (okCount) {
                bool okVals = out[0].getValue() == 10.0 && out[1].getValue() == 20.0;
                logExpect(okVals, failures, log, "Values are 10 and 20 in order");
            }
        }
    }

    // ---------- Test 4: source runs out mid-chunk (no spurious values) ----------
    // Expect exactly one valid value to appear after the latency; no fabricated extra valids.
    {
        Bus bus;
        FakeSource src("S4");
        bus.setSourceComponent(&src);
        bus.setBusWidth(3);

        src.push(42.0);

        // Cycle 1: pending <- (42); ready empty
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            logExpect(n == 0, failures, log, "Sparse input: still no output on first cycle");
        }

        // Cycle 2: ready <- (42)
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            bool okCount = (n == 1);
            logExpect(okCount, failures, log, "Exactly one value becomes available");
            if (okCount) {
                bool okVal = out[0].isValid() && out[0].getValue() == 42.0;
                logExpect(okVal, failures, log, "The value is 42");
            }
        }

        // Cycle 3: no new inputs, still empty
        bus.simulate();
        {
            std::vector<DataValue<DATA_TYPE>> out;
            int n = drainAll(bus, out);
            logExpect(n == 0, failures, log, "No further outputs without new inputs");
        }
    }

    // ---------- Test 5: getters/setters ----------
    {
        Bus bus;
        FakeSource src("S5");
        bus.setSourceComponent(&src);
        bus.setBusWidth(8);

        auto width = bus.getBusWidth();
        logExpect(width == 8u, failures, log, "getBusWidth() reflects setBusWidth(8)");
    }

    if (failures == 0) log << "All Bus tests PASSED." << std::endl;
    else log << failures << " Bus test(s) FAILED." << std::endl;
    log.close();
    return failures;
}