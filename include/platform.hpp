#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "component.hpp"
#include "bus.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "display.hpp"

#include <vector>

enum ComponentType {
    BUS_TYPE,
    MEMORY_TYPE,
    CPU_TYPE,
    DISPLAY_TYPE,
    PLATFORM_TYPE
};
class Platform : public Component {
private:
    std::vector<Component*> components;

public:
    void addComponent(Component* component);
    std::vector<Component*>& getComponents();
    std::string getFileName(const std::string path);
    ComponentType getTypeFromFileName(const std::string fileName);
    void loadComponents();
    void bindComponents();

    void simulate() override;
    void load() override;
};

#endif 