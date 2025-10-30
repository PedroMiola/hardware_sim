#include "../include/platform.hpp"
#include "../include/testUtils.hpp"
#include "../include/bus.hpp"
#include "../include/memory.hpp"
#include "../include/cpu.hpp"
#include "../include/display.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#define CONFIG_PATH "../data/"

static std::vector<std::string> readPlatformFileAsPaths(const std::string& path) {
    std::vector<std::string> paths;
    std::ifstream file(path);
    if (!file) {
        return paths;
    }

    std::string line;
    while (std::getline(file, line)) {
        const auto first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        const auto last = line.find_last_not_of(" \t\r\n");
        std::string trimmed = line.substr(first, last - first + 1);
        if (trimmed.empty()) continue;
        if (trimmed[0] == '#') continue;
        paths.push_back(trimmed);
    }
    return paths;
}

int main() {
    std::ofstream log("../log/outputPlatform.log");
    if (!log) {
        std::cerr << "Error: could not open ../log/outputPlatform.log" << std::endl;
        return 1;
    }

    int failures = 0;

    // ------------------------------------------------------------
    // Test 0: normal platform load
    // ------------------------------------------------------------
    {
        Platform platform;
        std::string cfgPath = CONFIG_PATH + std::string("platform1.txt");
        platform.setConfigFilePath(cfgPath);

        auto expectedPaths = readPlatformFileAsPaths(cfgPath);
        logExpect(!expectedPaths.empty(), failures, log, "platform1.txt read and has at least 1 valid entry");

        try {
            platform.load();
            logExpect(true, failures, log, "platform.load() did not throw (normal)");
        } catch (const std::exception& e) {
            logExpect(false, failures, log, std::string("platform.load() threw (normal): ") + e.what());
            log << failures << " Platform test(s) FAILED." << std::endl;
            return failures;
        }

        auto& comps = platform.getComponents();
        logExpect(comps.size() == expectedPaths.size(), failures, log, "Platform created the same number of components as there are lines in platform1.txt");

        for (unsigned int i = 0; i < expectedPaths.size() && i < comps.size(); ++i) {
            Component* c = comps[i];
            const std::string& expectedPath = expectedPaths[i];
            logExpect(c != nullptr, failures, log, "Component #" + std::to_string(i) + " is not null");

            if (c) logExpect(c->getConfigFilePath() == expectedPath, failures, log, "Component #" + std::to_string(i) + " config path == line in file (" + expectedPath + ")");

            std::string fileName = platform.getFileName(expectedPath);
            ComponentType type = platform.getTypeFromFileName(fileName);

            switch (type) {
                case BUS_TYPE: {
                    Bus* b = dynamic_cast<Bus*>(c);
                    logExpect(b != nullptr, failures, log, "Component #" + std::to_string(i) + " is a BUS (from " + fileName + ")");
                    break;
                }
                case MEMORY_TYPE: {
                    Memory* m = dynamic_cast<Memory*>(c);
                    logExpect(m != nullptr, failures, log, "Component #" + std::to_string(i) + " is a MEMORY (from " + fileName + ")");
                    break;
                }
                case CPU_TYPE: {
                    CPU* cpu = dynamic_cast<CPU*>(c);
                    logExpect(cpu != nullptr, failures, log, "Component #" + std::to_string(i) + " is a CPU (from " + fileName + ")");
                    break;
                }
                case DISPLAY_TYPE: {
                    Display* d = dynamic_cast<Display*>(c);
                    logExpect(d != nullptr, failures, log, "Component #" + std::to_string(i) + " is a DISPLAY (from " + fileName + ")");
                    break;
                }
                case PLATFORM_TYPE: {
                    Platform* p2 = dynamic_cast<Platform*>(c);
                    logExpect(p2 != nullptr, failures, log, "Component #" + std::to_string(i) + " is a nested PLATFORM (from " + fileName + ")");
                    break;
                }
                default:
                    logExpect(false, failures, log, "Component #" + std::to_string(i) + " has unknown type (from " + fileName + ")");
                    break;
            }
        }
    }

    // ------------------------------------------------------------
    // Test 1: testing bindings after load
    // ------------------------------------------------------------
    {
        Platform platform;
        std::string cfgPath = CONFIG_PATH + std::string("platform1.txt");
        platform.setConfigFilePath(cfgPath);

        try {
            platform.load();
            logExpect(true, failures, log, "platform.load() did not throw (binding test)");
        } catch (const std::exception& e) {
            logExpect(false, failures, log, std::string("platform.load() threw (binding test): ") + e.what());
            log << failures << " Platform test(s) FAILED." << std::endl;
            return failures;
        }

        try {
            platform.bindComponents();
            logExpect(true, failures, log, "platform.bindComponents() did not throw");
        } catch (const std::exception& e) {
            logExpect(false, failures, log, std::string("platform.bindComponents() threw: ") + e.what());
            log << failures << " Platform test(s) FAILED." << std::endl;
            return failures;
        }

        auto& comps = platform.getComponents();
        for (unsigned int i = 0; i < comps.size(); i++) {
            Component* c = comps[i];
            if (c->getSourceLabel() != "") {
                std::string sourceLabel = c->getSourceLabel();
                bool found = false;
                for (Component* other : comps) {
                    if (other->getLabel() == sourceLabel) {
                        found = true;
                        break;
                    }
                }
                logExpect(found, failures, log, "Component #" + std::to_string(i) + " source label '" + sourceLabel + "' is bound to an existing component");
            }
        }
    }

    if (failures == 0) log << "All Platform tests PASSED." << std::endl;
    else log << failures << " Platform test(s) FAILED." << std::endl;
    log.close();
    return failures;
}
