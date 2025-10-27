#include "../include/display.hpp"
#include "../include/testUtils.hpp"

#include <fstream>
#include <sstream>
#include <vector>

int main() {
    std::ofstream log("../log/outputDisplay.log");
    if (!log) {
        std::cerr << "Error: Unable to open ../log/outputDisplay.log\n";
        return 1;
    }

    int failures = 0;

    // ---------- Test 0: No output before first firing (refreshRate gating) ----------
    {
        Display disp;
        FakeSource src("D0");
        std::ostringstream oss;

        disp.setSourceComponent(&src);
        disp.setRefreshRate(3);          
        disp.setOutputStream(oss);

        src.push(1.0);
        src.push(2.0);
        src.push(3.0);

        disp.simulate(); 
        logExpect(oss.str().empty(), failures, log, "No output on tick 1 when refreshRate=3");

        disp.simulate();
        logExpect(oss.str().empty(), failures, log, "No output on tick 2 when refreshRate=3");

        disp.simulate();
        std::string out = oss.str();
        bool ok = (out == std::string("1\n2\n3\n"));
        logExpect(ok, failures, log, "Tick 3 outputs '1\\n2\\n3\\n' in order");
    }

    // ---------- Test 1: Reads ALL available values when it fires ----------
    {
        Display disp;
        FakeSource src("D1");
        std::ostringstream oss;

        disp.setSourceComponent(&src);
        disp.setRefreshRate(1);         
        disp.setOutputStream(oss);

        src.push(10.0);
        src.push(20.0);
        src.push(30.0);

        disp.simulate(); 
        std::string out = oss.str();
        bool ok = (out == std::string("10\n20\n30\n"));
        logExpect(ok, failures, log, "Fires with refreshRate=1 and drains all available values");
    }

    // ---------- Test 2: Fewer values after next firing ----------
    {
        Display disp;
        FakeSource src("D2");
        std::ostringstream oss;

        disp.setSourceComponent(&src);
        disp.setRefreshRate(2);         
        disp.setOutputStream(oss);

        src.push(5.0);
        src.push(6.0);

        disp.simulate(); // tick 1: no output yet
        logExpect(oss.str().empty(), failures, log, "No output on first tick (refreshRate=2)");

        disp.simulate(); // tick 2: outputs both values
        logExpect(oss.str() == std::string("5\n6\n"), failures, log, "Second tick outputs '5\\n6\\n'");

        oss.str("");
        oss.clear();
        src.push(7.0);

        disp.simulate(); // tick 3: no output (counter=1)
        logExpect(oss.str().empty(), failures, log, "No output on third tick (waiting for next fire)");

        disp.simulate(); // tick 4: outputs 7
        logExpect(oss.str() == std::string("7\n"), failures, log, "Fourth tick outputs '7\\n'");
    }

    // ---------- Test 3: Switching output stream at runtime ----------
    {
        Display disp;
        FakeSource src("D3");
        std::ostringstream oss1, oss2;

        disp.setSourceComponent(&src);
        disp.setRefreshRate(1);
        disp.setOutputStream(oss1);

        src.push(100.0);
        src.push(200.0);

        disp.simulate(); // should write to oss1
        logExpect(oss1.str() == std::string("100\n200\n"), failures, log, "Writes to initial stream (oss1)");
        logExpect(oss2.str().empty(), failures, log, "No writes to oss2 yet");

        // Switch 
        disp.setOutputStream(oss2);
        src.push(300.0);
        disp.simulate(); // should now write to oss2

        logExpect(oss2.str() == std::string("300\n"), failures, log, "After switching, writes go to oss2");
        logExpect(oss1.str() == std::string("100\n200\n"), failures, log, "Previous stream (oss1) remains unchanged");
    }

    // ---------- Test 4: Setters/Getters ----------
    {
        Display disp;
        FakeSource src("D4");
        std::ostringstream oss;

        disp.setSourceComponent(&src);
        disp.setOutputStream(oss);
        disp.setRefreshRate(5);

        logExpect(disp.getRefreshRate() == 5, failures, log, "getRefreshRate reflects setRefreshRate(5)");
    }

    if (failures == 0) log << "All Display tests PASSED." << std::endl;
    else log << failures << " Display test(s) FAILED." << std::endl;
    log.close();
    return failures;
}
