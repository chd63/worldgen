#ifndef CHUNK_H
#define CHUNK_H
#include <iostream>
#include <bits/stdc++.h>
#include <memory>
#include <math.h>

const float TRIANGLE_LENTH = 0.10f;
const float PLANE_LENGTH = 1.0f;
const int VERTEX_COUNT = 3; 

class Chunk{
public:
    std::unique_ptr<float[]> vertices;
    std::unique_ptr<unsigned int[]> indices;
    unsigned int indicesLength;
    unsigned int vertexLength;
    std::unordered_map<std::string, float> heightMap;
    glm::vec3 localOrigin;
    float** heightArray = nullptr;

    float triangleLength = 0.05f;
    float planeLength = 1.0f;

    // constructor( float triangleLength, float planeLength, origin, heightmap)
    //, float** noise2dArray
    Chunk(float trianglelength, float planelength, glm::vec3 origin)
    {   
        triangleLength = trianglelength;
        planeLength = planelength;
        localOrigin = origin;
        createChunk(triangleLength, planeLength);
    }

    Chunk(float trianglelength, float planelength, glm::vec3 origin, float** heightarray)
    {   
        triangleLength = trianglelength;
        heightArray = heightarray;
        planeLength = planelength;
        localOrigin = origin;
        createChunk(triangleLength, planeLength);
    }


private:
    // create chunk
    void createChunk(float tLength, float pLength )
    {
        unsigned int sideLength = (pLength / tLength) + 1;
        unsigned int vertex_index = 0;
        vertexLength = sideLength * sideLength  * (VERTEX_COUNT + 2);
        vertices = std::make_unique<float[]>(vertexLength);

        int textureCourner = 0;

        //col
        for(unsigned int i = 0; i < sideLength ; i++)
        {   
            // row
            for(unsigned int j = 0; j < sideLength ; j++)
            {
                vertices[vertex_index] = static_cast<float>(j * tLength);
                vertices[vertex_index + 1] = static_cast<float>(i * tLength);
                
                if(heightArray == nullptr)
                    vertices[vertex_index + 2] = 0.0f;
                else
                {
                    vertices[vertex_index + 2] = heightArray[(int)localOrigin.x + j][(int)localOrigin.y + i];
                    //std::cout << heightArray[(int)localOrigin.x + j][(int)localOrigin.y + i] << '\n';
                }

                vertices[vertex_index + 3] = static_cast<float>(j * tLength);
                vertices[vertex_index + 4] = static_cast<float>(i * tLength);

                std::string temp= std::to_string(i) + " " + std::to_string(j);
                if(heightArray == nullptr)
                    heightMap[temp] =  0.0f;
                else
                {
                    heightMap[temp] = heightArray[(int)localOrigin.x + j][(int)localOrigin.y + i];
                }

                vertex_index = vertex_index + VERTEX_COUNT + 2;
            }
        }

        indicesLength = (sideLength - 1) * (sideLength - 1) *  (2 * VERTEX_COUNT) ;
        unsigned int indicie_index = 0;
        indices = std::make_unique<unsigned int[]>(indicesLength);

        for(unsigned int i = 0; i < sideLength - 1; i++)
        {
            for(unsigned int j = 0; j < sideLength - 1; j++)
            {
                // tl, tr, bl, br
                unsigned int tl = (i + 1) * sideLength + j;
                unsigned int tr = tl + 1;
                unsigned int bl = i * sideLength + j;
                unsigned int br = bl + 1;

                // triangle 1
                indices[indicie_index] = tl;
                indices[indicie_index + 1] = bl;
                indices[indicie_index + 2] = br;
                indicie_index +=  VERTEX_COUNT;

                // triangle 2
                indices[indicie_index] = tl;
                indices[indicie_index + 1] = br;
                indices[indicie_index + 2] = tr;

                indicie_index +=  VERTEX_COUNT;
            }
        }
    }

};


#endif