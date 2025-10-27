#ifndef CPU_HPP
#define CPU_HPP

#include "component.hpp"
#include "dataValue.hpp"
#include <string>
#include <queue>
#include <list>
#include <tuple>

enum operation{NOP, ADD, SUB, MUL, DIV};

typedef std::tuple<operation,DATA_TYPE,DATA_TYPE> instruction;

class CPU : public ReadableComponent {
public:
    CPU(int frequency, int coreCount, const std::string& programPath);
    void load();

    int getFrequency() const;
    int getCoreCount() const;
    const std::string& getProgramPath() const;

    void setFrequency(int frequency);
    void setCoreCount(int coreCount);
    void setProgramPath(const std::string& programPath);

    void parseProgram();
    void execute_instruction(instruction inst);

    std::string readInfo() const;
    DataValue<DATA_TYPE> read();
    void simulate();

private:
    int frequency;
    int coreCount;
    std::string programPath;
    int activeCoreIndex;

    int instructionPointer;
    std::list<instruction> program;
    std::queue<DataValue<DATA_TYPE>> dataRegister;

};

#endif // CPU_HPP
