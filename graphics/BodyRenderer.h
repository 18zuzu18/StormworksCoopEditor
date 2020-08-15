//
// Created by minec on 15.08.2020.
//

#ifndef STROMWORKSCOOPEDITOR_BODYRENDERER_H
#define STROMWORKSCOOPEDITOR_BODYRENDERER_H

#include "SurfaceRenderer.h"
#include "../logic/Vehicle.h"

class BodyRenderer {

    SurfaceRenderer sr;
    Shader *shader;
    ComponentManager *cm;
public:

    explicit BodyRenderer(ComponentManager *cm) {
        this->cm = cm;
        shader = static_cast<Shader *>(malloc(sizeof(Shader)));
        *shader = Shader("../graphics/shaders/vertex.glsl", "../graphics/shaders/fragment.glsl");
        shader->use();
    }

    ~BodyRenderer() {
        delete shader;
    }

    void renderBody(Body *body, glm::mat4 projection, glm::mat4 view) {
        for (int i = 0; i < body->blocks.size(); ++i) {
            Block *block = &(body->blocks[i]);
            glm::vec3 pos = block->position;
            pos /= 4;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = model * glm::mat4(block->rotation);
            glm::to_string(model); // DONT REMOVE. IT BREAKS THE MOVEMENT
            glm::mat4 trans = projection * view * model;
            shader->use();
            shader->setMat4("transform", trans);
            Component *component = &(cm->components[block->componentId]);
            if (component->mesh.loaded) {
                shader->use();
                component->mesh.draw();
            }
            if (component->mesh0.loaded) {
                shader->use();
                component->mesh0.draw();
            }
            if (component->mesh1.loaded) {
                shader->use();
                component->mesh1.draw();
            }
            sr.renderSurfaces(component->surfaces, trans);
        }
    }

};

#endif //STROMWORKSCOOPEDITOR_BODYRENDERER_H
