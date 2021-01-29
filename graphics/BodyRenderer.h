//
// Created by minec on 15.08.2020.
//

#ifndef STROMWORKSCOOPEDITOR_BODYRENDERER_H
#define STROMWORKSCOOPEDITOR_BODYRENDERER_H

#include "SurfaceRenderer.h"
#include "../logic/Vehicle.h"
#include "Mesh.h"

class BodyRenderer {

    SurfaceRenderer sr;
    Shader *shader;
    ComponentManager *cm;
    std::vector<Mesh *> cachedMeshs;
public:

    explicit BodyRenderer(ComponentManager *cm) {
        cachedMeshs.resize(128);
        this->cm = cm;
        shader = static_cast<Shader *>(malloc(sizeof(Shader)));
        *shader = Shader("../graphics/shaders/vertex.glsl", "../graphics/shaders/fragment.glsl");
        shader->use();
    }

    ~BodyRenderer() {
        delete shader;
    }


    // Body to Mesh
    void updateBody(Body *body, unsigned int cacheId) {
        if (cacheId > 127) {
            std::cout << "Too many bodies" << std::endl;
            return;
        }
        makeMeshFromBody(body, cm, cacheId);
    }

    void makeMeshFromBody(Body *body, ComponentManager *cm, unsigned int cacheId) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned long tindiCount = 0;
        unsigned long indiCount = 0;
        for (int i = 0; i < body->blocks.size(); ++i) {
            Block *block = &(body->blocks[i]);
            Component *component = (cm->components[block->componentId]);
            if (component->mesh.loaded) {
                Mesh *cmesh = &component->mesh;
                // TODO Rotate Blocks
                for (int j = 0; j < cmesh->vertices.size(); ++j) {
                    Vertex v = cmesh->vertices[j];
                    glm::vec3 pos = block->position;
                    pos /= 4;
                    v.position += pos;
                    vertices.emplace_back(v);
                    tindiCount++;
                }
                for (int j = 0; j < cmesh->indices.size(); ++j) {
                    indices.emplace_back(cmesh->indices[j] + indiCount);
                }
            }
            //TODO Add Secondary meshes and Surfaces
            indiCount = tindiCount;
        }

        Mesh *m = static_cast<Mesh *>(malloc(sizeof(Mesh)));
        m = new Mesh;
        m->load(vertices, indices);
        cachedMeshs[cacheId] = m;
    }

    void renderBody(unsigned int body, glm::mat4 projection, glm::mat4 view) {
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
        pos /= 4;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        model = model;// * glm::mat4(block->rotation);
        glm::to_string(model); // DONT REMOVE. IT BREAKS THE MOVEMENT
        glm::mat4 trans = projection * view * model;
        shader->use();
        shader->setMat4("transform", trans);
        cachedMeshs[body]->draw();
//        for (int i = 0; i < body->blocks.size(); ++i) {
//            Block *block = &(body->blocks[i]);
//            glm::vec3 pos = block->position;
//            pos /= 4;
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::translate(model, pos);
//            model = model * glm::mat4(block->rotation);
//            glm::to_string(model); // DONT REMOVE. IT BREAKS THE MOVEMENT
//            glm::mat4 trans = projection * view * model;
//            shader->use();
//            shader->setMat4("transform", trans);
//            Component *component = (cm->components[block->componentId]);
//            if (component->mesh.loaded) {
//                shader->use();
//                component->mesh.draw();
//            }
//            if (component->mesh0.loaded) {
//                shader->use();
//                component->mesh0.draw();
//            }
//            if (component->mesh1.loaded) {
//                shader->use();
//                component->mesh1.draw();
//            }

//            sr.renderSurfaces(component->surfaces, trans);
// }
    }

};

#endif //STROMWORKSCOOPEDITOR_BODYRENDERER_H
