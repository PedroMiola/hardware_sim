#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "component.hpp"
/*#include "bus.hpp"
#include "memory.hpp"
#include "cpu.hpp"
#include "display.hpp"*/

#include <vector>

class Platform : public Component {
private:
    std::vector<Component*> components;

public:
    void addComponent(Component* component);
    std::vector<Component*>& getComponents();

    void simulate() override;
    void load() override;
};

#endif 