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

#include <pthread.h>
#include <unistd.h>

class ComponentManager {

private:
    static const uint8_t threadNum = 8; // min 2

public:
    Component **components;
    uint16_t numComponents;

    pthread_t threads[threadNum];

    struct threaddata {
        std::vector<std::filesystem::path> *paths;
        uint16_t start;
        uint16_t end;
        uint8_t id;
        ComponentManager *parent;
    };

    static void *loadComponent(void *ipath) {
        threaddata data = *(threaddata *) (ipath);

        std::vector<std::filesystem::path> *paths = data.paths;

        for (int i = data.start; i < data.end; ++i) {
            Component *component = new Component((*paths)[i].string());
            component->filename = (*paths)[i].filename().string();
            data.parent->components[i] = component;
        }
        free(ipath);
    }

    void loadComponents() {
        auto start = std::chrono::steady_clock::now();
        std::cout << "Loading Components" << std::endl;

        std::vector<std::filesystem::path> paths;

        for (const auto &entry : std::filesystem::directory_iterator(gamepath + "data/definitions/")) {
            paths.emplace_back(entry.path());
        }
        paths.shrink_to_fit();

        numComponents = paths.size();
        components = (Component **) malloc(sizeof(Component) * numComponents);

//        std::cout << "ALL: " << paths.size() << std::endl;
//        std::cout << "0-14: " << paths.size() / (threadNum - 1) << std::endl;
//        std::cout << "(threadNum-1): " << paths.size() - (paths.size() / (threadNum - 1) * (threadNum - 1))
//                  << std::endl;

        // Thread 1-14
        for (int i = 0; i < threadNum - 1; ++i) {
            threaddata *data = (threaddata *) malloc(sizeof(threaddata));
            data->paths = &paths;
            data->start = i * (paths.size() / (threadNum - 1));
            data->end = (i + 1) * (paths.size() / (threadNum - 1));
            data->id = i;
            data->parent = this;
            pthread_create(&threads[i], NULL, &ComponentManager::loadComponent, (void *) data);
            //sleep(1);
        }
        // Thread (threadNum-1)
        threaddata *data = (threaddata *) malloc(sizeof(threaddata));
        data->paths = &paths;
        data->start = (paths.size() / (threadNum - 1) * (threadNum - 1));
        data->end = paths.size();
        data->id = threadNum - 1;
        data->parent = this;
        pthread_create(&threads[threadNum - 1], NULL, &ComponentManager::loadComponent, (void *) data);
        sleep(1);

        // Join all Threads
        for (int i = 0; i < threadNum; ++i) {
            pthread_join(threads[i], NULL);
        }

        // Load Meshes into GPU
        for (int i = 0; i < numComponents; ++i) {
            components[i]->loadMeshes();
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "\nLoaded " << numComponents << " Components in: " << diff.count() << " s\n\n";
    }

    int getComponentIdbyFilename(std::string filename) {
        filename += ".xml";
        for (int i = 0; i < numComponents; ++i) {
            if (components[i]->filename == filename) {
                return i;
            }
        }
        return 0;
    }

    int getComponentIdbyName(std::string name) {
        for (int i = 0; i < numComponents; ++i) {
            if (components[i]->filename == name) {
                return i;
            }
        }
        return 0;
    }

    Component *getComponentbyFilename(std::string filename) {
        filename += ".xml";
        for (int i = 0; i < numComponents; ++i) {
            if (components[i]->filename == filename) {
                return *(components + i);
            }
        }
    }

    Component *getComponentbyName(std::string name) {
        for (int i = 0; i < numComponents; ++i) {
            if (components[i]->filename == name) {
                return *(components + i);
            }
        }
    }

};

#endif //STROMWORKSCOOPEDITOR_COMPONENTMANAGER_H
