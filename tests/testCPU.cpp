#include "../include/cpu.hpp"
#include <iostream>
#include <fstream>

int main() {
    // 1. Create a small example program file dynamically
    std::ofstream prog("test_program.txt");
    prog << "ADD 2 3\n";
    prog << "SUB 10 4\n";
    prog << "MUL 3 5\n";
    prog << "DIV 8 2\n";
    prog << "NOP 0 0\n";
    prog.close();

    try {
        // 2. Instantiate CPU (frequency = 2, 2 cores)
        CPU cpu(2, 2, "test_program.txt");

        std::cout << "=== CPU CREATED ===" << std::endl;
        std::cout << cpu.readInfo() << std::endl;

        // 3. Run several simulation steps
        for (int step = 0; step < 6; ++step) {
            std::cout << "\n--- Simulation Step " << step + 1 << " ---" << std::endl;
            cpu.simulate();

            // 4. Read all available values in register
            while (true) {
                DataValue<DATA_TYPE> val = cpu.read();
                if (!val.isValid()) break;
                std::cout << "Read value: " << val.getValue() << std::endl;
            }
        }

        std::cout << "\nSimulation finished." << std::endl;
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

