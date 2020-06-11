//
// Created by jens on 09.06.20.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include "mesh/MeshDecoder.h"
#include "graphics/Shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void processInput(GLFWwindow *window);

unsigned int windowWidth = 800;
unsigned int windowHeight = 600;

glm::vec3 orbitpoint(0.0f, 0.0f, 0.0f);
glm::vec3 rotation;
float zoom = 2;

int main() {

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    MeshDecoder::decodeMesh("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/meshes/prop_microscope.mesh", &vertices, &indices);

    unsigned int vsize = vertices.size();
    for (int i = 0; i < indices.size(); ++i) {
//        std::cout << indices[i] << " ";
        indices[i] = vsize - 1 - indices[i];
//        std::cout << indices[i] << " ";
        if ((i + 1) % 3 == 0) {
//            std::cout << std::endl;
        }
    }

    std::cout << "Hello Stormworkers" << std::endl;
    // GLFW Init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // GLFW Window Creation
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL with Glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set Viewport and FramebufferSizeCallback
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    Shader shader("../graphics/shaders/vertex.glsl", "../graphics/shaders/fragment.glsl");
    shader.use();

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int EBO;
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

    glEnable(GL_DEPTH_TEST);

    rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    // Game Loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//        model = glm::rotate(model, glm::radians((float) glfwGetTime() * 25), glm::vec3(1.0f, 1.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
//        view = glm::translate(view, glm::vec3(cosf((float) glfwGetTime()), sinf((float) glfwGetTime()), -2.0f));
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

//        view = glm::translate(view, orbitpoint);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -zoom));
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(-1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//        view = glm::rotate(view, glm::radians(0.0f), rotation);

//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float) windowWidth / (float) windowHeight, 0.1f, 100.0f);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = projection * model * view;
        shader.setMat4("transform", trans);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Cleanup
    glfwTerminate();
    return 0;
}

float lastX = 400, lastY = 300;
bool firstMouse = true;
float sensitivity = 0.1f;
bool rotating = false;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    if (rotating) {
        rotation.y += xoffset * sensitivity;
        rotation.x += yoffset * sensitivity;
//        rotation = glm::rotate(rotation, glm::radians(yoffset * sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));
//        rotation = glm::rotate(rotation, glm::radians(xoffset * sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetMouseButton(window, 1)) {
        rotating = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        rotating = false;
        firstMouse = true;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}