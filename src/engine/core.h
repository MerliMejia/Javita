#ifndef __CORE_H__
#define __CORE_H__

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <deque>

#include "shader.h"

namespace Javita
{
    void run();
    void finish();
    void addOnUpdateCallback(std::function<void(float)>);
    struct Transform
    {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 getModelMatrix()
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, scale);
            return model;
        }
    };

    struct Rendeable
    {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        Shader shader;
        Javita::Transform transform = Javita::Transform();
        glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.2f);

        void init()
        {
            shader = createShader("defaultShader.vs", "defaultShader.fs");

            glUseProgram(shader.shaderProgram);

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            std::vector<float> transformedVertices;
            transformedVertices.reserve(vertices.size() + (vertices.size() / 3 * 3)); // Reserve space for efficiency

            for (size_t i = 0; i < vertices.size(); i += 3)
            {
                // Copy position
                transformedVertices.push_back(vertices[i]);
                transformedVertices.push_back(vertices[i + 1]);
                transformedVertices.push_back(vertices[i + 2]);

                // Append color values
                transformedVertices.push_back(color.r);
                transformedVertices.push_back(color.g);
                transformedVertices.push_back(color.b);
            }

            float *verticesData = transformedVertices.data();

            glBufferData(GL_ARRAY_BUFFER, transformedVertices.size() * sizeof(float), verticesData, GL_DYNAMIC_DRAW);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            unsigned int *indicesData = indices.data();

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indicesData, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        void draw()
        {
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
    };

    namespace Render
    {
        Rendeable *createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices);
        Rendeable *createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec3 color);

        namespace Primitives
        {
            Rendeable *createTriangle();
            Rendeable *createQuad();
        } // namespace Primitives

    } // namespace Render

} // namespace Javita

#endif // __CORE_H__