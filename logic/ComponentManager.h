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
        for (const auto &entry : std::filesystem::directory_iterator("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/data/definitions/")) {
            Component component(entry.path());
//            for (int i = 0; i < component.logicNodes.size(); ++i) {
//                std::cout << component.logicNodes[i].label << std::endl;
//            }
//            std::cout << entry.path() << std::endl;
            std::cout << component.name << std::endl;
            components.emplace_back(component);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Loaded Components in: " << diff.count() << " s\n\n";
    }
};

#endif //STROMWORKSCOOPEDITOR_COMPONENTMANAGER_H
