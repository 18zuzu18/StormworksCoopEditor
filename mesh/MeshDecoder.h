//
// Created by jens on 10.06.20.
//

#ifndef STROMWORKSCOOPEDITOR_MESHDECODER_H
#define STROMWORKSCOOPEDITOR_MESHDECODER_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct Vertex {
    Vertex(glm::vec3 position, glm::vec4 color, glm::vec3 normal) {
        this->position = position;
        this->color = color;
        this->normal = normal;
    }

    Vertex() {}

    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normal;
};

class MeshDecoder {
private:
    static float decodeFloat(std::vector<unsigned char> buffer, unsigned int pos) {
        float f;
        unsigned char b[] = {buffer[pos - 3], buffer[pos - 2], buffer[pos - 1], buffer[pos - 0]};
        std::memcpy(&f, &b, sizeof(f));
        return f;
    }

public:
    static void decodeMesh(std::string path, std::vector<Vertex> *vertices, std::vector<unsigned int> *indices) {
        // Load File
//        std::ifstream input( "/home/jens/.steam/steam/steamapps/common/Stormworks/rom/meshes/grid_square_voxel.mesh", std::ios::binary );
//        std::ifstream input("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/meshes/unit_cube_centred.mesh", std::ios::binary);
//        std::ifstream input("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/meshes/oil_rig_derrick.mesh", std::ios::binary);
//        std::ifstream input("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/meshes/unit_sphere.mesh", std::ios::binary);
        std::ifstream input(path, std::ios::binary);
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

        // Num Vertex
        unsigned int numVertex = buffer[0x08] | buffer[0x09] << 8;
        std::cout << "Num Vertex: ";
        std::cout << numVertex << std::endl;

        // End of Vertex
        unsigned int endOfVertex = 13 + numVertex * 28;
        std::cout << "End of Vertex: 0x";
        printf("%08x", endOfVertex);
        std::cout << std::endl;
        std::cout << "Start of Vertex: 0x";
        printf("%08x", 0x0E);
        std::cout << std::endl << std::endl;

        (*vertices).resize(numVertex);

        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < numVertex; ++i) {
            int vertexStart = endOfVertex - i * 28;
            (*vertices)[i].color = glm::vec4(buffer[vertexStart - 15] / 255.0f, buffer[vertexStart - 14] / 255.0f, buffer[vertexStart - 13] / 255.0f, buffer[vertexStart - 12] / 255.0f);
            (*vertices)[i].position = glm::vec3(decodeFloat(buffer, vertexStart - 16), decodeFloat(buffer, vertexStart - 20), decodeFloat(buffer, vertexStart - 24));;
            (*vertices)[i].normal = glm::vec3(decodeFloat(buffer, vertexStart), decodeFloat(buffer, vertexStart - 4), decodeFloat(buffer, vertexStart - 8));
            if ((*vertices)[i].color * 255.0f == glm::vec4(109.000000, 160.000000, 199.000000, 255.000000)) {
                (*vertices)[i].color.a = 0.0f;
            }
            if ((i % 1000) == 0) {
                std::cout << (float) i / (float) numVertex * 100.0f << "%" << std::endl;
            }
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Loaded Vertices in: " << diff.count() << " s\n\n";

        // Index
        unsigned int a = int((unsigned char) (buffer[numVertex * 28 + 16]) << 16 |
                             (unsigned char) (buffer[numVertex * 28 + 15]) << 8 |
                             (unsigned char) (buffer[numVertex * 28 + 14]));
        unsigned int numIndex = a;
        std::cout << "Num Index: ";
        std::cout << numIndex << std::endl;

        unsigned int startIndex = numVertex * 28 + 18;
        std::cout << "Start Index: 0x";
        printf("%08x", startIndex);
        std::cout << std::endl << std::endl;

        (*indices).resize(numIndex);

        for (int j = 0; j < numIndex; ++j) {
            unsigned int index = buffer[startIndex + j * 2 + 0] | buffer[startIndex + j * 2 + 1] << 8;
            if (index > numVertex) {
                printf("%08x", startIndex + j * 2 + 1);
                std::cout << "HELP" << std::endl;
            }
            (*indices)[j] = index;
        }
        unsigned int vsize = (*vertices).size();
        for (int i = 0; i < (*indices).size(); ++i) {
//        std::cout << indices[i] << " ";
            (*indices)[i] = vsize - 1 - (*indices)[i];
//        std::cout << indices[i] << " ";
            if ((i + 1) % 3 == 0) {
//            std::cout << std::endl;
            }
        }
    }

private:
    // Disallow creating an instance of this object
    MeshDecoder() {}
};

#endif //STROMWORKSCOOPEDITOR_MESHDECODER_H
