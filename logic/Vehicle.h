//
// Created by minec on 15.08.2020.
//

#ifndef STROMWORKSCOOPEDITOR_VEHICLE_H
#define STROMWORKSCOOPEDITOR_VEHICLE_H

#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <rapidxml.hpp>
#include <filesystem>
#include <rapidxml_utils.hpp>
#include <iostream>
#include "ComponentManager.h"

struct Block {
    glm::vec3 position;
    glm::mat3 rotation;
    int mirror;
    glm::vec4 baseColor;
    glm::vec4 adaptiveColor;
    int componentId;

    //bool hasCustomName = false;
    //std::string customName;
};

struct Body {
    int id;
    glm::mat4 initialLocalTransform;
    glm::mat4 localTransform;

    std::vector<Block> blocks;
};

void outputAllAttributes(rapidxml::xml_node<> *node) {
    for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        std::cout << attr->name() << ": " << attr->value() << std::endl;
    }
}

void outputAllNodes(rapidxml::xml_node<> *node) {
    for (rapidxml::xml_node<> *attr = node->first_node(); attr; attr = attr->next_sibling()) {
        std::cout << attr->name() << std::endl;
    }
}

void loadMat4(rapidxml::xml_node<> *node, glm::mat4 *output) {
    *output = glm::mat4(
            atof(node->first_attribute("00")->value()),
            atof(node->first_attribute("01")->value()),
            atof(node->first_attribute("02")->value()),
            atof(node->first_attribute("03")->value()),

            atof(node->first_attribute("10")->value()),
            atof(node->first_attribute("11")->value()),
            atof(node->first_attribute("12")->value()),
            atof(node->first_attribute("13")->value()),

            atof(node->first_attribute("20")->value()),
            atof(node->first_attribute("21")->value()),
            atof(node->first_attribute("22")->value()),
            atof(node->first_attribute("23")->value()),

            atof(node->first_attribute("30")->value()),
            atof(node->first_attribute("31")->value()),
            atof(node->first_attribute("32")->value()),
            atof(node->first_attribute("33")->value())
    );
}

class Vehicle {

public:

    bool isAdvanced = false;
    bool isStatic = false;

    std::vector<Body> bodies;

    explicit Vehicle(std::string path, ComponentManager cm) {
        rapidxml::file<> xmlFile(path.c_str()); // Default template is char
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());
        rapidxml::xml_node<> *vehicle = doc.first_node("vehicle");
        rapidxml::xml_node<> *bodiesNode = vehicle->first_node("bodies");
        // Loop Bodies
        for (rapidxml::xml_node<> *child = bodiesNode->first_node(); child; child = child->next_sibling()) {
            Body body;
            body.id = atoi(child->first_attribute("unique_id")->value());
            // Transforms
            loadMat4(child->first_node("local_transform"), &(body.localTransform));
            loadMat4(child->first_node("initial_local_transform"), &(body.initialLocalTransform));
            // Blocks
            rapidxml::xml_node<> *components = child->first_node("components");
            for (rapidxml::xml_node<> *blockNode = components->first_node(); blockNode; blockNode = blockNode->next_sibling()) {
                rapidxml::xml_node<> *o = blockNode->first_node("o");
                rapidxml::xml_node<> *vp = o->first_node("vp");

                Block block;
                // Id
                std::string componentName = blockNode->first_attribute("d")->value();
                block.componentId = cm.getComponentIdbyFilename(componentName);
                block.mirror = atoi(blockNode->first_attribute("t")->value());
                // Position
                block.position = glm::vec3(atof(vp->first_attribute("x")->value()),
                                           atof(vp->first_attribute("y")->value()),
                                           atof(vp->first_attribute("z")->value()));
                // Rotation
                std::string rotString = o->first_attribute("r")->value();
                int values[9];
                std::string delimiter = ",";

                int i = 0;
                size_t pos = 0;
                std::string token;
                while ((pos = rotString.find(delimiter)) != std::string::npos) {
                    token = rotString.substr(0, pos);
                    values[i] = atoi(token.c_str());
                    ++i;
                    rotString.erase(0, pos + delimiter.length());
                }
                values[8] = atoi(rotString.c_str());
                block.rotation = glm::mat3(1.0f);
                if (block.mirror == 0)
                    block.rotation = glm::mat3(values[6], values[7], values[8],
                                               values[3], values[4], values[5],
                                               values[0], values[1], values[2]);
                if (block.mirror == 1)
                    block.rotation = glm::mat3(values[6], values[7], values[8],
                                               values[3], values[4], values[5],
                                               -values[0], -values[1], -values[2]);
                if (block.mirror == 2)
                    block.rotation = glm::mat3(values[6], values[7], values[8],
                                               -values[3], -values[4], -values[5],
                                               values[0], values[1], values[2]);
                if (block.mirror == 3)
                    block.rotation = glm::mat3(values[6], values[7], values[8],
                                               -values[3], -values[4], -values[5],
                                               -values[0], -values[1], -values[2]);
                if (block.mirror == 4)
                    block.rotation = glm::mat3(-values[6], -values[7], -values[8],
                                               values[3], values[4], values[5],
                                               values[0], values[1], values[2]);
                if (block.mirror == 5)
                    block.rotation = glm::mat3(-values[6], -values[7], -values[8],
                                               values[3], values[4], values[5],
                                               -values[0], -values[1], -values[2]);
                if (block.mirror == 6)
                    block.rotation = glm::mat3(-values[6], -values[7], -values[8],
                                               -values[3], -values[4], -values[5],
                                               values[0], values[1], values[2]);
                if (block.mirror == 7)
                    block.rotation = glm::mat3(-values[6], -values[7], -values[8],
                                               -values[3], -values[4], -values[5],
                                               -values[0], -values[1], -values[2]);
                // Colors
                unsigned char baseC[4];
                sscanf(o->first_attribute("bc")->value(), "%02hhx%02hhx%02hhx%02hhx", &baseC[0], &baseC[1], &baseC[2],
                       &baseC[3]);
                block.baseColor = glm::vec4(baseC[0], baseC[1], baseC[2], baseC[3]);
                unsigned char addiC[4];
                sscanf(o->first_attribute("ac")->value(), "%02hhx%02hhx%02hhx%02hhx", &addiC[0], &addiC[1], &addiC[2],
                       &addiC[3]);
                block.adaptiveColor = glm::vec4(addiC[0], addiC[1], addiC[2], addiC[3]);

                body.blocks.emplace_back(block);
            }
            bodies.emplace_back(body);
        }
    }

};

#endif //STROMWORKSCOOPEDITOR_VEHICLE_H

/*
 * block.rotation = glm::mat3(values[6], values[7], values[8],
                                           values[3], values[4], values[5],
                                           values[0], values[1], values[2]);
 */