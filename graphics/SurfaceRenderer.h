//
// Created by minec on 10.07.2020.
//

#ifndef STROMWORKSCOOPEDITOR_SURFACERENDERER_H
#define STROMWORKSCOOPEDITOR_SURFACERENDERER_H

#include "../logic/Component.h"
#include "../mesh/MeshDecoder.h"
#include "Mesh.h"
#include "Shader.h"

class SurfaceRenderer {

    /*
     * 5 Up
     * 4 Front
     * 3 Down
     * 2 Back
     * 1 Left
     * 0 Right
     */
    std::vector<glm::vec2> orientations = {
            glm::vec2(0.0f, 0.0f), // Front
            glm::vec2(180.0f, 0.0f), // Back
            glm::vec2(-90.0f, 0.0f), // Up
            glm::vec2(90.0f, 0.0f), // Down
            glm::vec2(-90.0f, 90.0f), // Right
            glm::vec2(-90.0f, -90.0f)  // Left
    };
    std::vector<float> rotations = {
            0.0f,
            -90.0f,
            180.0f,
            90.0f
    };

    std::vector<std::vector<Vertex>> vertices = {
            {},
            {
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, -0.125f, 0.125f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, 0.125f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0, 0, 1))
            },
            {
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, 0.125f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1))
            },
            {},
            {},
            {},
            {
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, -0.125f, -0.125f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, -0.125f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0, 0, 1))
            },
            {
                    Vertex(glm::vec3(0.125f, -0.125f, -0.125f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, -0.125f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1))
            },
            {
                    Vertex(glm::vec3(0.125f, -0.125f, 0.125f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, -0.125f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0, 0, 1))
            }
    };
    std::vector<std::vector<unsigned int>> indices = {
            {},
            {
                    0, 1, 2,
                    1, 2, 3
            },
            {
                    0, 1, 2
            },
            {},
            {},
            {},
            {
                    0, 1, 2,
                    1, 2, 3
            },
            {
                    0, 1, 2
            },
            {
                    0, 1, 2
            }
    };
    std::vector<Mesh> meshes;

    Shader *s;

public:

    explicit SurfaceRenderer() {
        meshes.resize(indices.size());
        for (int i = 0; i < indices.size(); ++i) {
            meshes[i].load(vertices[i], indices[i]);
        }
        s = new Shader("../graphics/shaders/vertexSurface.glsl", "../graphics/shaders/fragmentSurface.glsl");
    }

    void renderSurface(Surface *surface, glm::mat4 transform) {
        glm::mat4 rotation(1.0f);
        float t = (float) glfwGetTime() * 30.0f;
        // Position
        glm::vec3 npos(surface->position.z, surface->position.y, surface->position.x);
        rotation = glm::translate(rotation, npos/4.0f);
        // Orientate
        rotation = glm::rotate(rotation, glm::radians(orientations[surface->orientation].x),
                               glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(orientations[surface->orientation].y),
                               glm::vec3(0.0f, 1.0f, 0.0f));
        // Rotate
        rotation = glm::rotate(rotation, glm::radians(rotations[surface->rotation]), glm::vec3(0.0f, 0.0f, 1.0f));
        // Shape
        if (surface->shape < meshes.size()) {
            Mesh *mesh = &(meshes[surface->shape]);
            s->use();
            s->setMat4("transform", transform);
            s->setMat4("rotation", rotation);
            mesh->draw();
        } else {
            // TODO Implement other Shapes
        }
    }

    void renderSurfaces(std::vector<Surface> surfaces, glm::mat4 transform) {
        for (int i = 0; i < surfaces.size(); ++i) {
            renderSurface(&(surfaces[i]), transform);
        }
    }

};

#endif //STROMWORKSCOOPEDITOR_SURFACERENDERER_H
