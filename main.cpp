//
// Created by jens on 09.06.20.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rapidxml_utils.hpp"

#include <iostream>
#include <chrono>

#include "mesh/MeshDecoder.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "logic/Component.h"
#include "logic/ComponentManager.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

unsigned int windowWidth = 800;
unsigned int windowHeight = 600;

glm::vec3 orbitpoint(0.0f, 0.0f, 0.0f);
glm::vec3 rotation;
float zoom = 2;

int main() {



    std::cout << "Hello Stormworkers" << std::endl;
    // GLFW Init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 4);

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

    glEnable(GL_MULTISAMPLE);
    // Set Viewport and FramebufferSizeCallback
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader shader("../graphics/shaders/vertex.glsl", "../graphics/shaders/fragment.glsl");
    shader.use();

//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//
//    MeshDecoder::decodeMesh("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/" + wedge.meshDataName, &vertices, &indices);
//
//    Mesh mesh;
//    mesh.load(vertices, indices);
//
//    std::vector<Vertex> vertices2;
//    std::vector<unsigned int> indices2;
//
//    MeshDecoder::decodeMesh("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/" + wedge.mesh0Name, &vertices2, &indices2);
//
//    Mesh mesh2;
//    mesh2.load(vertices2, indices2);

    ComponentManager cm;
    cm.loadComponents();

    Component wedge("/home/jens/.steam/steam/steamapps/common/Stormworks/rom/data/definitions/inventory_outfit_parachute.xml");
    std::cout << wedge.meshDataName << std::endl;

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
        // TODO Orbitpoint

        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -zoom));
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(-1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, -orbitpoint);
//        view = glm::rotate(view, glm::radians(0.0f), rotation);

//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(70.0f), (float) windowWidth / (float) windowHeight, 0.1f, 1000.0f);

        glm::mat4 trans = glm::mat4(1.0f);
        trans = projection * view * model;
        shader.setMat4("transform", trans);

        wedge.mesh.draw();

        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        trans = projection * view * model;
        shader.setMat4("transform", trans);
        for (int i = 0; i < cm.components.size(); ++i) {
            if (cm.components[i].mesh.loaded) {
                cm.components[i].mesh.draw();
            }
            model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
            trans = projection * view * model;
            shader.setMat4("transform", trans);
        }

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
bool moving = false;

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
        if (rotation.x > 90.0f) {
            rotation.x = 90.0f;
        } else if (rotation.x < -90.0f) {
            rotation.x = -90.0f;
        }
//        rotation = glm::rotate(rotation, glm::radians(yoffset * sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));
//        rotation = glm::rotate(rotation, glm::radians(xoffset * sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));
    } else if (moving) {
//        glm::vec3 direction;
//        direction.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
//        direction.y = sin(glm::radians(rotation.y));
//        direction.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
        glm::vec3 direction;
        direction.x = cos(glm::radians(rotation.y)); // Note that we convert the angle to radians first
        direction.z = sin(glm::radians(rotation.y));
        glm::vec3 ydirection;
        ydirection.x = sin(glm::radians(-rotation.x)) * direction.z; // Note that we convert the angle to radians first
        ydirection.y = cos(glm::radians(rotation.x));
        ydirection.z = sin(glm::radians(rotation.x)) * direction.x;
        glm::vec3 cameraRight = glm::normalize(direction);
//        glm::vec3 cameraFront = glm::normalize(glm::cross(cameraRight, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 cameraUp = glm::normalize(ydirection);
//        std::cout << glm::to_string(cameraUp) << std::endl;


        float cameraSpeed = 2.5f / 60.0f / 50.0f * zoom / ((float) windowWidth / 1920.0f);
        orbitpoint -= cameraSpeed * cameraUp * yoffset;
        orbitpoint -= cameraSpeed * cameraRight * xoffset;
    }
}

float scrollBase = 1.1f;
float minZoom = 0.5f;
float maxZoom = 250.0f;

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    zoom *= powf(scrollBase, -yoffset);
    if (zoom < minZoom)
        zoom = minZoom;
    if (zoom > maxZoom)
        zoom = maxZoom;
}

void processInput(GLFWwindow *window) {
    if (glfwGetMouseButton(window, 1)) {
        rotating = true;
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        rotating = false;
    }
    if (glfwGetMouseButton(window, 2)) {
        moving = true;
    } else {
        moving = false;
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