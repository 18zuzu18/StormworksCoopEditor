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
#define COLOR

#ifdef COLOR
#define RED glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define BLUE glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
#define GREEN glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define PINK glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)
#define CYAN glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
#endif
#ifndef COLOR
#define RED glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define BLUE glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define GREEN glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define PINK glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define CYAN glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#endif

    std::vector<std::vector<Vertex>> vertices = {
            {}, // 0
            {   // 1
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, -0.125f, 0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, 0.125f), RED, glm::vec3(0, 0, 1))
            },
            {   // 2
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, 0.125f), CYAN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), PINK, glm::vec3(0, 0, 1))
            },
            {   // 3
#define SurfaceRenderer_large 0.0625f
#define SurfaceRenderer_small 0.025888f
                    Vertex(glm::vec3(0.0f, 0.0f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(SurfaceRenderer_large, SurfaceRenderer_small, 0.125f), GREEN, glm::vec3(0, 0, 1)),  // A
                    Vertex(glm::vec3(SurfaceRenderer_large, -SurfaceRenderer_small, 0.125f), PINK, glm::vec3(0, 0, 1)), // H
                    Vertex(glm::vec3(-SurfaceRenderer_large, SurfaceRenderer_small, 0.125f), BLUE, glm::vec3(0, 0, 1)), // D
                    Vertex(glm::vec3(-SurfaceRenderer_large, -SurfaceRenderer_small, 0.125f), RED, glm::vec3(0, 0, 1)),// E
                    Vertex(glm::vec3(SurfaceRenderer_small, SurfaceRenderer_large, 0.125f), GREEN, glm::vec3(0, 0, 1)),  // B
                    Vertex(glm::vec3(-SurfaceRenderer_small, SurfaceRenderer_large, 0.125f), PINK, glm::vec3(0, 0, 1)), // C
                    Vertex(glm::vec3(SurfaceRenderer_small, -SurfaceRenderer_large, 0.125f), BLUE, glm::vec3(0, 0, 1)), // G
                    Vertex(glm::vec3(-SurfaceRenderer_small, -SurfaceRenderer_large, 0.125f), RED, glm::vec3(0, 0, 1)) // F
#undef SurfaceRenderer_large
#undef SurfaceRenderer_small
            },
            {}, // 4
            {}, // 5
            {   // 6
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, -0.125f, -0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, -0.125f), RED, glm::vec3(0, 0, 1))
            },
            {   // 7
                    Vertex(glm::vec3(0.125f, -0.125f, -0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, -0.125f), CYAN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), PINK, glm::vec3(0, 0, 1))
            },
            {   // 8
                    Vertex(glm::vec3(0.125f, -0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, -0.125f), CYAN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), PINK, glm::vec3(0, 0, 1))
            },
            {   // 9
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.0f, -0.125f, 0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, 0.125f), RED, glm::vec3(0, 0, 1))
            },
            {   // 10
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.0f, 0.125f, 0.125f), RED, glm::vec3(0, 0, 1))
            },
            {   // 11
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.0f, 0.125f, 0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, -0.125f, 0.125f), RED, glm::vec3(0, 0, 1))
            },
            {   // 12
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.0f, -0.125f, 0.125f), RED, glm::vec3(0, 0, 1))
            },
            {}, // 13
            {}, // 14
            {}, // 15
            {}, // 16
            {}, // 17
            {}, // 18
            {}, // 19
            {}, // 20
            {   // 21
                    Vertex(glm::vec3(0.0f, 0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, 0.125f, -0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.0f, -0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.125f, -0.125f, -0.125f), RED, glm::vec3(0, 0, 1))
            },
            {   // 22
                    Vertex(glm::vec3(-0.125f, 0.125f, 0.125f), GREEN, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.0f, 0.125f, -0.125f), PINK, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(-0.125f, -0.125f, 0.125f), BLUE, glm::vec3(0, 0, 1)),
                    Vertex(glm::vec3(0.0f, -0.125f, -0.125f), RED, glm::vec3(0, 0, 1))
            }
#undef RED
#undef GREEN
#undef BLUE
#undef PINK
#undef CYAN
#ifndef COLOR
#undef COLOR
#endif
    };
    // grid_square_voxel.mesh
    std::vector<std::vector<unsigned int>> indices = {
            {}, // 0
            {   // 1
                    0, 1, 2,
                    1, 2, 3
            },
            {   // 2
                    0, 1, 2
            },
            {   // 3
                    0, 1, 2,
                    0, 3, 4,
                    0, 5, 6,
                    0, 7, 8,
                    0, 1, 5,
                    0, 3, 6,
                    0, 4, 8,
                    0, 2, 7
            },
            {}, // 4
            {}, // 5
            {   // 6
                    0, 1, 2,
                    1, 2, 3
            },
            {   // 7
                    0, 1, 2
            },
            {   // 8
                    0, 1, 2
            },
            {   // 9
                    0, 1, 2,
                    1, 2, 3
            },
            {   // 10
                    0, 1, 2
            },
            {   // 11
                    0, 1, 2,
                    1, 2, 3
            },
            {   // 12
                    0, 1, 2
            },
            {}, // 13
            {}, // 14
            {}, // 15
            {}, // 16
            {}, // 17
            {}, // 18
            {}, // 19
            {}, // 20
            {   // 21
                    0, 1, 2,
                    1, 2, 3
            },
            {   // 22
                    0, 1, 2,
                    1, 2, 3
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
        rotation = glm::translate(rotation, npos / 4.0f);
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
