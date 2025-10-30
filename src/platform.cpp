#include "platform.hpp"

void Platform::addComponent(Component* component) {
    components.push_back(component);
}

std::vector<Component*>& Platform::getComponents() {
    return components;
}

void Platform::simulate() {
    for (Component* component : components) {
        component->simulate();
    }
}

std::string Platform::getFileName(const std::string path){
    if (path.empty()) return "";

    unsigned int end = path.find_last_not_of("/\\");
    if (end == std::string::npos) return "";

    std::string trimmed = path.substr(0, end + 1);

    unsigned int sep = trimmed.find_last_of("/\\");
    std::string file = (sep == std::string::npos) ? trimmed : trimmed.substr(sep + 1);

    unsigned int dot = file.find_last_of('.');
    if (dot == std::string::npos || dot == 0) return file;

    return file.substr(0, dot);
}

ComponentType Platform::getTypeFromFileName(const std::string fileName) {
    unsigned int i = fileName.size();
    while (i > 0 && isdigit(fileName[i - 1])) {
        --i;
    }
    std::string type_string = fileName.substr(0, i);
    if (type_string == "bus") {
        return BUS_TYPE;
    } else if (type_string == "memory" || type_string == "mem") {
        return MEMORY_TYPE;
    } else if (type_string == "cpu") {
        return CPU_TYPE;
    } else if (type_string == "display") {
        return DISPLAY_TYPE;
    } else if (type_string == "platform") {
        return PLATFORM_TYPE;
    } else {
        throw std::runtime_error("Unknown component type in file name: " + fileName);
    }
}

void Platform::load() {
    
    std::ifstream file(configFilePath);
    if (!file) {
        throw std::runtime_error("Cannot open configuration file: " + configFilePath);
    }

    std::string line;
    while (std::getline(file, line)) {

        const auto first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) continue;
        const auto last = line.find_last_not_of(" \t\r\n");
        std::string path = line.substr(first, last - first + 1);

        if (path.empty() || path[0] == '#') continue;

        std::string fileName = getFileName(path);
        ComponentType type = getTypeFromFileName(fileName);

        if(type == BUS_TYPE){
            Bus* bus = new Bus();
            bus->setConfigFilePath(path);
            addComponent(bus);
        } else if(type == MEMORY_TYPE){
            Memory* memory = new Memory();
            memory->setConfigFilePath(path);
            addComponent(memory);
        } else if(type == CPU_TYPE){
            CPU* cpu = new CPU();
            cpu->setConfigFilePath(path);
            addComponent(cpu);
        } else if(type == DISPLAY_TYPE){
            Display* display = new Display();
            display->setConfigFilePath(path);
            addComponent(display);
        } else if(type == PLATFORM_TYPE){
            Platform* platform = new Platform();
            platform->setConfigFilePath(path);
            addComponent(platform);
        }
    }

    loadComponents();
}

void Platform::loadComponents(){
    for (Component* component : components) {
        component->load();
    }
}

void Platform::bindComponents(){
    for(auto component: components){
        if(component->getSourceLabel() == "") continue;
        for(auto potentialSource: components){
            if(potentialSource->getLabel() == component->getSourceLabel()){
                ReadableComponent* readableSource = dynamic_cast<ReadableComponent*>(potentialSource);
                if(auto bus = dynamic_cast<Bus*>(component)){
                    bus->setSourceComponent(readableSource);
                } else if(auto memory = dynamic_cast<Memory*>(component)){
                    memory->setSourceComponent(readableSource);
                } else if(auto display = dynamic_cast<Display*>(component)){
                    display->setSourceComponent(readableSource);
                }
            }
        }
    }
}
