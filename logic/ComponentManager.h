//
// Created by jens on 14.06.20.
//

#ifndef STROMWORKSCOOPEDITOR_COMPONENTMANAGER_H
#define STROMWORKSCOOPEDITOR_COMPONENTMANAGER_H

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include "Component.h"

class ComponentManager {
public:
    std::vector<Component> components;

    void loadComponents() {
        auto start = std::chrono::steady_clock::now();
        std::cout << "Loading Components" << std::endl;
        int i = 0;
        for (const auto &entry : std::filesystem::directory_iterator(gamepath + "data/definitions/")) {
            Component component(entry.path().string());
            ++i;
            if(i%50 == 0){
                std::cout << ".";
            }
            component.filename = entry.path().filename().string();
            components.emplace_back(component);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "\nLoaded Components in: " << diff.count() << " s\n\n";
    }

    int getComponentIdbyFilename(std::string filename){
        filename += ".xml";
        for (int i = 0; i < components.size(); ++i) {
            if (components[i].filename == filename) {
                return i;
            }
        }
        return 0;
    }

    int getComponentIdbyName(std::string name){
        for (int i = 0; i < components.size(); ++i) {
            if (components[i].filename == name) {
                return i;
            }
        }
        return 0;
    }

    Component* getComponentbyFilename(std::string filename){
        filename += ".xml";
        for (int i = 0; i < components.size(); ++i) {
            if (components[i].filename == filename) {
                return components.data() + i;
            }
        }
    }

    Component* getComponentbyName(std::string name){
        for (int i = 0; i < components.size(); ++i) {
            if (components[i].filename == name) {
                return components.data() + i;
            }
        }
    }

};

#endif //STROMWORKSCOOPEDITOR_COMPONENTMANAGER_H
