#ifndef WORLDGEN_H
#define WORLDGEN_H
#include <iostream>
#include <bits/stdc++.h>

struct Plane{
    float *vertices;
    unsigned int *indices;
    unsigned int indices_length;
    unsigned int vertex_length;
    // getto way should use something better
    std::unordered_map<std::string ,float> heightMap; 
};

const float TRIANGLE_LENTH = 0.10f;
const float PLANE_LENGTH = 1.0f;
const int VERTEX_COUNT = 3; 

float lerp(float point1, float point2, float x);
float** createValueNoise(unsigned int sideLength =  8192, unsigned int step = 16, float distroLow = 0.0f, float distroHigh = 1.0f );
Plane createPlaneThroughTriangles(float tLength = 0.10f , float pLength = 1.0f);


Plane createPlaneThroughTriangles(float tLength, float pLength ){
    Plane plane;

    // we should send specific information that we want like starting origin 
    // length of a triangle/square and finally length of a plane

    // how is noise going to be introduced?
    // well a big noise 

    unsigned int sideLength = (pLength / tLength) + 1;
    unsigned int vertex_index = 0;
    plane.vertex_length = sideLength * sideLength  * (VERTEX_COUNT + 2);
    plane.vertices = new float[plane.vertex_length];

    // load noise 
    float **noiseArray;
    noiseArray = createValueNoise(8192, 256, -10.0, 50.0);

    int textureCourner = 0;

    //col
    for(unsigned int i = 0; i < sideLength ; i++)
    {   
        // row
        for(unsigned int j = 0; j < sideLength ; j++)
        {
            plane.vertices[vertex_index] = static_cast<float>(j * tLength);
            plane.vertices[vertex_index + 1] = static_cast<float>(i * tLength);
            //plane.vertices[vertex_index + 2] = 0.0f;
            plane.vertices[vertex_index + 2] = noiseArray[i * 4][j * 4];

            // this is for the texture so every three verties we have to put a diffrent section
            // off the texture
            plane.vertices[vertex_index + 3] = static_cast<float>(j * tLength);
            plane.vertices[vertex_index + 4] = static_cast<float>(i * tLength);

            std::string temp= std::to_string(i) + " " + std::to_string(j);
            plane.heightMap[temp] =  noiseArray[i * 4][j * 4];

            // todo add vetices for texture
            vertex_index = vertex_index + VERTEX_COUNT + 2;
        }
    }

    plane.indices_length = (sideLength - 1) * (sideLength - 1) *  (2 * VERTEX_COUNT) ;
    unsigned int indicie_index = 0;
    plane.indices = new unsigned int[plane.indices_length];

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
            plane.indices[indicie_index] = tl;
            plane.indices[indicie_index + 1] = bl;
            plane.indices[indicie_index + 2] = br;
            indicie_index +=  VERTEX_COUNT;

            // triangle 2
            plane.indices[indicie_index] = tl;
            plane.indices[indicie_index + 1] = br;
            plane.indices[indicie_index + 2] = tr;

            indicie_index +=  VERTEX_COUNT;
        }
    }
 
    return plane;
}

// YOU WILL BE UNABLE TO HAVE FILLED IN OBJECTS PROPERLY WITH THIS APPROCH, YOU WILL HAVE A MESH
// YOU CAN GET A GETTO VERSION OF FILLED OBJECTS BY USING GL_TRIANGLE_STRIP
// TEXTURES ARE ALSO A NIGHTMARE TO INTRODUCE
// ALSO A BUG WHERE THE LARGER THE NUMBER OF PLANE LENGTH / TRIANGLE_LENGTH EXTRA LINES IMPLEMENTED 
// INTO THE PLANE
// THIS IS MOSTLY STILL IN THE CODE BECUASE I THINK ITS NEAT AND I SPENT TOO MUCH TIME ON IT

Plane createPlaneThroughLines(float tLength = TRIANGLE_LENTH , float pLength = PLANE_LENGTH)
{
    Plane plane;

    // set random gen
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distribution(0.0, 0.1);    


    // TODO: FIX VARIABLE NAMES THEY ARE FUCKING SHIT
    // TODO:CHANGE NAME OF VETEXT LENGTH
    int vertexLength = (pLength / tLength) + 1;
    unsigned int vertex_index = 0;
    unsigned int vertex_length = vertexLength * vertexLength * VERTEX_COUNT;

    plane.vertex_length = vertex_length;
    plane.vertices = new float[plane.vertex_length];

    // load noise 
    float **noiseArray;
    //noiseArray = createValueNoise(256, 12);

    // so we need to create an array of vertieces that 
    // will be connectect by and array od indices
    for(unsigned int i = 0; i < vertexLength; i++)
    {
        for(unsigned int j = 0; j < vertexLength; j++)
        {
            plane.vertices[vertex_index] = static_cast<float>(j * tLength);
            plane.vertices[vertex_index + 1] = static_cast<float>(i * tLength);
            plane.vertices[vertex_index + 2] = 0.0f;
            //plane.vertices[vertex_index + 2] = noiseArray[i][j];
            vertex_index = vertex_index + VERTEX_COUNT;

            //std::cout << static_cast<float>(j * tLength) << "        " << static_cast<float>(i * tLength) << '\n';
        }
    }

    // TODO: FIX THIS SHIT CODE 
    // two loops for indices
    unsigned int outside_length = ((vertexLength - 1) * 4 ) + 1;
    unsigned int inside_length = (((vertexLength - 1) * 2) - 1 ) * (vertexLength - 1)
        + ((vertexLength - 1) * (vertexLength - 2));

    //plane.indices_length = outside_length + inside_length;
    plane.indices_length = outside_length + inside_length;
    plane.indices = new unsigned int[plane.indices_length];

    
    unsigned int aggregate_number = 0;
    unsigned int tracker = 0;
    int trackerCase = 0;

    // loop to create outside links
    for(int index = 0; index < outside_length; index++)
    {

        switch(trackerCase)
        {
            case 1:
                plane.indices[aggregate_number] = tracker;
                //std::cout << tracker << '\n';
                tracker = tracker + vertexLength;
                break;
            case 2:
                plane.indices[aggregate_number] = tracker;
                //std::cout << tracker << '\n';
                tracker = tracker - 1;
                break;
            case 3:
                plane.indices[aggregate_number] = tracker;
                //std::cout << tracker << '\n';
                tracker = tracker - vertexLength;
                break;
            default:
                plane.indices[aggregate_number] = tracker;
                //std::cout << tracker << '\n';
                tracker++;

        }

        if(tracker == vertexLength - 1)
            trackerCase = 1;
        if(tracker == vertexLength * vertexLength - 1)
            trackerCase = 2;
        if(tracker == (vertexLength * vertexLength) - vertexLength )
            trackerCase = 3;

        aggregate_number++;
    }
    
    //std::cout << "------------------------------" << '\n';

    tracker = 0;
    trackerCase = 0;
    int col_counter = 0;
    // loop to create inside links
    while(aggregate_number < plane.indices_length )
    {

        switch(trackerCase)
        {
            case 1:
                plane.indices[aggregate_number] = tracker + vertexLength;
                //std::cout << tracker + vertexLength << '\n';
                aggregate_number++;
                plane.indices[aggregate_number] = tracker;
                //std::cout << tracker << '\n';
                col_counter++;
                tracker++;
                break;
            case 2:
                plane.indices[aggregate_number] = tracker;
                //std::cout << tracker << '\n';
                tracker = tracker - 1;
                col_counter = col_counter - 1;
                break;

        }

        if(col_counter == vertexLength - 1)
        {
            tracker = tracker + vertexLength;
            trackerCase = 2;
        }
        if(col_counter == 0)
        {
            plane.indices[aggregate_number] = tracker;
            //std::cout << tracker << '\n';
            col_counter++;
            tracker++;
            trackerCase = 1;
        }
        aggregate_number++;
    }    
    


    return plane;
}


float** createValueNoise(unsigned int sideLength , unsigned int step , float distroLow , float distroHigh)
{

    // large step few grid points, smooth
    // small step lots of grid points, bumpy
    const int randomSideLength = sideLength / step + 1;

    // another data structure might be better to use
    float randomNumberArray[randomSideLength][randomSideLength];

    // set random gen -> this can be improved
    std::default_random_engine gen;
    std::uniform_real_distribution<float> distribution(distroLow, distroHigh);  

    // create a 2d array that will have random values for each point
    for(int i = 0; i < randomSideLength; i++)
    {
        for(int j = 0; j < randomSideLength; j++)
        {
            randomNumberArray[i][j] = distribution(gen);
        }
    }

    float** finilizedArray = new float*[sideLength];

    for(int y = 0; y < sideLength; y++)
    {
        finilizedArray[y] = new float[sideLength];
        for(int x = 0; x < sideLength; x++)
        {
            // get grid cell coordinates
            int gx = x / step;
            int gy = y / step;

            // local position of cell (0-1)
            // median position so tx is center of x axis
            // and ty is center of y axis
            float tx = (x % step) / float(step); 
            float ty = (y % step ) / float(step);

            // ccorner values
            float v00 = randomNumberArray[gx][gy];
            float v10 = randomNumberArray[gx + 1][gy ];
            float v01 = randomNumberArray[gx][gy + 1];
            float v11 = randomNumberArray[gx + 1][gy+ 1];

            // interpolate horizontal then vertical
            float ix0 = lerp(v00 , v10 , tx);
            float ix1 = lerp(v01 , v11 , tx);
            float value = lerp(ix0, ix1, ty);

            finilizedArray[y][x] = value;
        }
    }

    return finilizedArray;
}

// a + t * (b - a)
float lerp(float a, float b, float t)
{
    // apply smoothstep
    // 6x^5 - 15x^4 + 10x^3
    t = t * t * t * (t * (t * 6 - 15) + 10);

    return a + t * (b - a);
}

#endif