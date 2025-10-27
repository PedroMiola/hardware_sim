#include "../include/dataValue.hpp"

#include <iostream>
#include <fstream>

int main ()
{
    std::ofstream logFile("../log/outputDataValue.log"); 
    if (!logFile) {
        std::cerr << "Error: Unable to open log file." << std::endl;
        return 1;
    }

    int testResult = 0;

    DataValue<int> dv1; 
    DataValue<int> dv2(42); 
    logFile << "dv1 is valid (expected '0'): " << dv1.isValid() << ", value (expected '0'): " << dv1.getValue() << std::endl;
    if(dv1.isValid() != 0 || dv1.getValue() != 0)
        testResult++;
    logFile << "dv2 is valid (expected '1'): " << dv2.isValid() << ", value (expected '42'): " << dv2.getValue() << std::endl;
    if(dv2.isValid() != 1 || dv2.getValue() != 42)
        testResult++;
    dv1.setValue(100);
    logFile << "After setting dv1, is valid (expected '1'): " << dv1.isValid() << ", value (expected '100'): " << dv1.getValue() << std::endl;
    if(dv1.isValid() != 1 || dv1.getValue() != 100)
        testResult++;
    dv2.unset();
    logFile << "After unsetting dv2, is valid (expected '0'): " << dv2.isValid() << std::endl;
    if(dv2.isValid() != 0)
        testResult++;

    logFile.close(); 
    return testResult;
}