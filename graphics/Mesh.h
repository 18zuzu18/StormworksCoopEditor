//
// Created by jens on 14.06.20.
//

#ifndef STROMWORKSCOOPEDITOR_MESH_H
#define STROMWORKSCOOPEDITOR_MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "../mesh/MeshDecoder.h"

class Mesh {
public:

    bool loaded = false;

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    unsigned int indicesCount;

    void loadfromFile(std::string path) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        MeshDecoder::decodeMesh(path, &vertices, &indices);
        load(vertices, indices);
    }

    void load(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {

        indicesCount = indices.size();

        glGenBuffers(1, &VBO);

        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &EBO);

        // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
        // 1. bind Vertex Array Object
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10 * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *) (7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        loaded = true;
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

};

#endif //STROMWORKSCOOPEDITOR_MESH_H
