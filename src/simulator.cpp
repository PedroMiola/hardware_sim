#include "../include/platform.hpp"
#include "../include/testUtils.hpp"
#include "../include/bus.hpp"
#include "../include/memory.hpp"
#include "../include/cpu.hpp"
#include "../include/display.hpp"

int main(int argc, char** argv) {
    // Recives the path for the paltform configuration file as argument and the number of simulation steps to execute.
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <platform_config_path> <simulation_steps>" << std::endl;
        return 1;
    }
    std::string platformConfigPath = argv[1];
    int simulationSteps = std::stoi(argv[2]);

    Platform platform;
    platform.setConfigFilePath(platformConfigPath);
    try {
        platform.load();
        platform.bindComponents();
    } catch (const std::exception& e) {
        std::cerr << "Error loading platform: " << e.what() << std::endl;
        return 1;
    }

    for (int step = 0; step < simulationSteps; step++) {
        platform.simulate();
    }

    return 0;
}