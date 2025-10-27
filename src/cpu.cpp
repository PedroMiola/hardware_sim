#include "../include/cpu.hpp"

CPU::CPU(int freq, int corec, const std::string& programp) // set frequency, core count and program path, parsing the file and building the program
    : frequency(freq),
      coreCount(corec),
      programPath(programp),
      activeCoreIndex(0),
      instructionPointer(0) 
{
    parseProgram();
}

void CPU::load() {
	std::cout <<"initialized CPU!" << std::endl;
}

DataValue<DATA_TYPE> CPU::read() {
    if (dataRegister.empty()) {
        return DataValue<DATA_TYPE>(); // invalid value
    } else {
        DataValue<DATA_TYPE> val = dataRegister.front();
        dataRegister.pop();
        return val;
    }
}

void CPU::simulate() {
    // Execute 'frequency' instructions per simulation step
    for (int i = 0; i < frequency; i++) {
        if (instructionPointer >= (int)program.size()) {
            // If we reached the end of the program
            if (activeCoreIndex < coreCount - 1) {
                // Move to next core and restart the program
                activeCoreIndex++;
                instructionPointer = 0;
            } else {
                std::cout << "All cores done: stop simulation" << std::endl;
                return;
            }
        }

        auto it = std::next(program.begin(), instructionPointer);
        execute_instruction(*it);
        instructionPointer++;
    }
}

void CPU::execute_instruction(instruction inst) {
    operation op;
    DATA_TYPE op1, op2;
    std::tie(op, op1, op2) = inst;

    
    DATA_TYPE result = 0.0f;

    switch (op) {
        case NOP: result = 0.0f; break;
        case ADD: result = op1 + op2; break;
        case SUB: result = op1 - op2; break;
        case MUL: result = op1 * op2; break;
        case DIV: result = (op2 != 0) ? op1 / op2 : 0.0f; break;
        default: result = 0.0f; break;
    }

    DataValue<DATA_TYPE> returnValue (result);
    dataRegister.push(returnValue);
}

void CPU::parseProgram() {
    std::ifstream file(programPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open program file: " + programPath);
    }

    program.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string opStr;
        float op1 = 0, op2 = 0;
        if (!(iss >> opStr >> op1 >> op2)) continue;

        operation opCode = NOP;
        if (opStr == "ADD") opCode = ADD;
        else if (opStr == "SUB") opCode = SUB;
        else if (opStr == "MUL") opCode = MUL;
        else if (opStr == "DIV") opCode = DIV;
        else opCode = NOP;

        program.push_back(std::make_tuple(opCode, op1, op2));
    }

    file.close();
}

std::string CPU::readInfo() const {
    std::ostringstream oss;
    oss << "CPU Info - Frequency: " << frequency
        << ", Cores: " << coreCount
        << ", Active core: " << activeCoreIndex
        << ", Program path: " << programPath;
    return oss.str();
}

// setters / getters  
void CPU::setFrequency(int freq) { frequency = freq; }
int CPU::getFrequency() const { return frequency; }

void CPU::setCoreCount(int coreC) { coreCount = coreC; }
int CPU::getCoreCount() const { return coreCount; }

void CPU::setProgramPath(const std::string& path) { programPath = path; }
const std::string& CPU::getProgramPath() const { return programPath; }
