#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <memory>
#include <math.h>
#include <algorithm>

template<typename T>
class Object
{
public:
    std::unique_ptr<T[]> vertexArray;
    std::unique_ptr<unsigned int[]> indicesArray;
    glm::vec3 worldPoint;
    int vertexLength;
    int indicesLength;
    unsigned int VBO, VAO, EBO;

    int offsetPerShape = 3;
    int offsetPerTexture = 2;
    int totalOffset = offsetPerShape + offsetPerTexture;

    Object(){}

    Object(std::unique_ptr<T[]> vertexarray, unsigned int vertexlength, std::unique_ptr<unsigned int[]> indicesarray, unsigned int indiceslength, glm::vec3 worldpoint)
    {
        vertexArray = std::move(vertexarray);
        vertexLength = vertexlength;
        indicesArray = std::move(indicesarray);
        indicesLength = indiceslength;
        worldPoint = worldpoint;
    } 

    Object(std::unique_ptr<T[]> vertexarray, unsigned int vertexlength, std::unique_ptr<unsigned int[]> indicesarray, unsigned int indiceslength, glm::vec3 worldpoint,
        int offsetpershape, int offsetpertexture)
    {
        vertexArray = std::move(vertexarray);
        vertexLength = vertexlength;
        indicesArray = std::move(indicesarray);
        indicesLength = indiceslength;
        worldPoint = worldpoint;
        offsetPerShape = offsetpershape;
        offsetPerTexture = offsetpertexture;
        totalOffset = offsetpershape + offsetpertexture;

    } 

    Object(T* vertexarray, unsigned int vertexlength, unsigned int* indicesarray, unsigned int indiceslength, glm::vec3 worldpoint,
        int offsetpershape, int offsetpertexture)
    {
        vertexArray = std::make_unique<T[]>(vertexlength);
        std::copy(vertexarray, vertexarray + vertexlength, vertexArray.get());
        
        indicesArray = std::make_unique<unsigned int[]>(indiceslength);
        std::copy(indicesarray, indicesarray + indiceslength, indicesArray.get());
        vertexLength = vertexlength;
        indicesLength = indiceslength;
        worldPoint = worldpoint;
        offsetPerShape = offsetpershape;
        offsetPerTexture = offsetpertexture;
        totalOffset = offsetpershape + offsetpertexture;

    } 

    void createBuffers()
    {   
        //set the buffers -> this might need to be sworked on when I have better understanding
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexLength * sizeof(T), vertexArray.get(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength * sizeof(unsigned int), indicesArray.get(), GL_STATIC_DRAW);

        // position attribute -> this will have to be edited and changed depending on object
        glVertexAttribPointer(0, offsetPerShape, GL_FLOAT, GL_FALSE, totalOffset * sizeof(T), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, offsetPerTexture, GL_FLOAT, GL_FALSE, totalOffset * sizeof(T), (void*)(offsetPerShape * sizeof(T)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

    }

    void deleteBuffers()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:

};

#endif