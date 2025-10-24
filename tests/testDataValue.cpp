#include "../include/dataValue.hpp"

#include <iostream>
#include <fstream>

int main ()
{
    std::ofstream logFile("../log/output.log"); 
    if (!logFile) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        return 1;
    }

    DataValue<int> dv1; 
    DataValue<int> dv2(42); 
    logFile << "dv1 is valid (expected '0'): " << dv1.isValid() << ", value (expected '0'): " << dv1.getValue() << std::endl;
    logFile << "dv2 is valid (expected '1'): " << dv2.isValid() << ", value (expected '42'): " << dv2.getValue() << std::endl;
    dv1.setValue(100);
    logFile << "After setting dv1, is valid (expected '1'): " << dv1.isValid() << ", value (expected '100'): " << dv1.getValue() << std::endl;
    dv2.unset();
    logFile << "After unsetting dv2, is valid (expected '0'): " << dv2.isValid() << std::endl;

    logFile.close(); 
    return 0;
}