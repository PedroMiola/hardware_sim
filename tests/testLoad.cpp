#include "../include/bus.hpp"
#include "../include/cpu.hpp"
#include "../include/display.hpp"
#include "../include/memory.hpp"
#include "../include/testUtils.hpp"

#include <iostream>

#define CONFIG_PATH "../data/"

int main(){
    std::ofstream log("../log/outputLoad.log");
    if (!log) {
        std::cerr << "Error: Unable to open ../log/outputLoad.log" << std::endl;
        return 1;
    }

    int failures = 0;

    // ---------- Test 0: testing normal bus load ----------
    {
        Bus bus;
        bus.setConfigFilePath(CONFIG_PATH + std::string("bus1.txt"));

        auto cfg = bus.parseKeyValues();

        try {
            bus.load();
            logExpect(true, failures, log, "bus.load() did not throw");
        } catch (const std::exception &e) {
            logExpect(false, failures, log, std::string("bus.load() threw: ") + e.what());
            return failures;
        }

        // LABEL
        if (cfg.count("LABEL")) logExpect(bus.getLabel() == cfg["LABEL"][0], failures, log, "Bus label matches file (" + bus.getLabel() + ")");
        else logExpect(false, failures, log, "Bus file is missing LABEL");

        // TYPE
        if (cfg.count("TYPE")) logExpect(bus.getType() == cfg["TYPE"][0], failures, log, "Bus type matches file (" + bus.getType() + ")");
        else logExpect(false, failures, log, "Bus file is missing TYPE");

        // WIDTH 
        if (cfg.count("WIDTH")) {
            unsigned int expected = std::stoul(cfg["WIDTH"][0]);
            unsigned int actual = bus.getBusWidth();
            logExpect(actual == expected, failures, log, "Bus width matches file (" + std::to_string(actual) + ")");
        } else logExpect(false, failures, log, "Bus file is missing BUS_WIDTH");

        // SOURCE
        if (cfg.count("SOURCE")) {
            std::string expected = cfg["SOURCE"][0];
            std::string actual = bus.getSourceLabel();
            logExpect(actual == expected, failures, log, "Bus source label matches file (" + actual + ")");
        } else logExpect(false, failures, log, "Bus file is missing SOURCE");
    }
    
    // ---------- Test 1: testing bus load with white spaces----------
    {
        Bus bus;
        bus.setConfigFilePath(CONFIG_PATH + std::string("bus1_whitespaces.txt"));

        auto cfg = bus.parseKeyValues();
        
        try {
            bus.load();
            logExpect(true, failures, log, "bus.load() did not throw");
        } catch (const std::exception &e) {
            logExpect(false, failures, log, std::string("bus.load() threw: ") + e.what());
            return failures;
        }

        // LABEL
        if (cfg.count("LABEL")) logExpect(bus.getLabel() == cfg["LABEL"][0], failures, log, "Bus label matches file (" + bus.getLabel() + ")");
        else logExpect(false, failures, log, "Bus file is missing LABEL");

        // TYPE
        if (cfg.count("TYPE")) logExpect(bus.getType() == cfg["TYPE"][0], failures, log, "Bus type matches file (" + bus.getType() + ")");
        else logExpect(false, failures, log, "Bus file is missing TYPE");

        // WIDTH 
        if (cfg.count("WIDTH")) {
            unsigned int expected = std::stoul(cfg["WIDTH"][0]);
            unsigned int actual = bus.getBusWidth();
            logExpect(actual == expected, failures, log, "Bus width matches file (" + std::to_string(actual) + ")");
        } else logExpect(false, failures, log, "Bus file is missing WIDTH");

        // SOURCE
        if (cfg.count("SOURCE")) {
            std::string expected = cfg["SOURCE"][0];
            std::string actual = bus.getSourceLabel();
            logExpect(actual == expected, failures, log, "Bus source label matches file (" + actual + ")");
        } else logExpect(false, failures, log, "Bus file is missing SOURCE");
    }

    // ---------- Test 2: testing bus load with wrong config ----------
    {
        Bus bus;
        bus.setConfigFilePath(CONFIG_PATH + std::string("bus1_wrong.txt"));

        try {
            bus.load();
            logExpect(false, failures, log, "bus.load() did not throw on wrong config");
        } catch (const std::exception &e) {
            logExpect(true, failures, log, std::string("bus.load() correctly threw: ") + e.what());
        }
    }

    // ---------- Test 3: testing normal memory load ----------
    {
        Memory memory;
        memory.setConfigFilePath(CONFIG_PATH + std::string("mem1.txt"));

        auto cfg = memory.parseKeyValues();

        try {
            memory.load();
            logExpect(true, failures, log, "memory.load() did not throw");
        } catch (const std::exception &e) {
            logExpect(false, failures, log, std::string("memory.load() threw: ") + e.what());
            return failures;
        }

        // LABEL
        if (cfg.count("LABEL")) logExpect(memory.getLabel() == cfg["LABEL"][0], failures, log, "Memory label matches file (" + memory.getLabel() + ")");
        else logExpect(false, failures, log, "Memory file is missing LABEL");

        // TYPE
        if (cfg.count("TYPE")) logExpect(memory.getType() == cfg["TYPE"][0], failures, log, "Memory type matches file (" + memory.getType() + ")");
        else logExpect(false, failures, log, "Memory file is missing TYPE");

        // SIZE
        if (cfg.count("SIZE")) {
            int expected = std::stoi(cfg["SIZE"][0]);
            int actual = memory.getSize();
            logExpect(actual == expected, failures, log, "Memory size matches file (" + std::to_string(actual) + ")");
        } else logExpect(false, failures, log, "Memory file is missing SIZE");

        // ACCESS_TIME
        if (cfg.count("ACCESS")) {
            int expected = std::stoi(cfg["ACCESS"][0]);
            int actual = memory.getAccessTime();
            logExpect(actual == expected, failures, log, "Memory access time matches file (" + std::to_string(actual) + ")");
        } else logExpect(false, failures, log, "Memory file is missing ACCESS_TIME");

        // SOURCE
        if (cfg.count("SOURCE")) {
            std::string expected = cfg["SOURCE"][0];
            std::string actual = memory.getSourceLabel();
            logExpect(actual == expected, failures, log, "Memory source label matches file (" + actual + ")");
        } else logExpect(false, failures, log, "Memory file is missing SOURCE");
    }

    // ---------- Test 4: testing memory load with wrong config ----------
    {
        Memory memory;
        memory.setConfigFilePath(CONFIG_PATH + std::string("mem1_wrong.txt"));

        try {
            memory.load();
            logExpect(false, failures, log, "memory.load() did not throw on wrong config");
        } catch (const std::exception &e) {
            logExpect(true, failures, log, std::string("memory.load() correctly threw: ") + e.what());
        }
    }

    // ---------- Test 5: testing normal display load ----------
    {
        Display display;
        display.setConfigFilePath(CONFIG_PATH + std::string("display1.txt"));

        auto cfg = display.parseKeyValues();

        try {
            display.load();
            logExpect(true, failures, log, "display.load() did not throw");
        } catch (const std::exception &e) {
            logExpect(false, failures, log, std::string("display.load() threw: ") + e.what());
            return failures;
        }

        // TYPE
        if (cfg.count("TYPE")) logExpect(display.getType() == cfg["TYPE"][0], failures, log, "Display type matches file (" + display.getType() + ")");
        else logExpect(false, failures, log, "Display file is missing TYPE");

        // REFRESH_RATE
        if (cfg.count("REFRESH")) {
            unsigned int expected = static_cast<unsigned int>(std::stoul(cfg["REFRESH"][0]));
            unsigned int actual = display.getRefreshRate();
            logExpect(actual == expected, failures, log, "Display refresh rate matches file (" + std::to_string(actual) + ")");
        } else logExpect(false, failures, log, "Display file is missing REFRESH");

        // SOURCE
        if (cfg.count("SOURCE")) {
            std::string expected = cfg["SOURCE"][0];
            std::string actual = display.getSourceLabel();
            logExpect(actual == expected, failures, log, "Display source label matches file (" + actual + ")");
        } else logExpect(false, failures, log, "Display file is missing SOURCE");

        // LABEL 
        if (cfg.count("LABEL")) logExpect(display.getLabel() == cfg["LABEL"][0], failures, log, "Display label matches file (" + display.getLabel() + ")");
        else logExpect(display.getLabel() == "Display 1", failures, log, "Display label defaulted to \"Display 1\"");
    }

    

    if (failures == 0) log << "All Load tests PASSED." << std::endl;
    else log << failures << " Load test(s) FAILED." << std::endl;
    log.close();
    return failures;
}