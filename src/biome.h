#ifndef BIOME_H
#define BIOME_H
#include <iostream>
#include <bits/stdc++.h>
#include <memory>
#include <math.h>

class Biome{
public:
    // 2d array of noise

    // MAKE SURE THIS IS SAFELY DELETED WHENEVER YOU USE IT
    // MAKE SURE THIS IS SAFELY DELETED WHENEVER YOU USE IT
    // MAKE SURE THIS IS SAFELY DELETED WHENEVER YOU USE IT
    // MAKE SURE THIS IS SAFELY DELETED WHENEVER YOU USE IT
    // MAKE SURE THIS IS SAFELY DELETED WHENEVER YOU USE IT
    // values are flipped rn I should probably fix this
    float** biomeData = nullptr;

    glm::vec2 origin;
    unsigned int sideLength;
    unsigned int step;
    float distroLow;
    float distroHigh;

    Biome()
        : biomeData(nullptr),
          origin(0.0f, 0.0f),
          sideLength(0),
          step(1),
          distroLow(0.0f),
          distroHigh(0.0f)
    {}

    // contructor
    Biome(unsigned int sidelength , unsigned int stepA , float distrolow , float distrohigh)
    {
        sideLength = sidelength;
        step = stepA;
        distroLow = distrolow;
        distroHigh = distrohigh;

        // will call noise wich will edit the 2d array
        biomeData = createNoise();
    }

private:

    //TODO :FIX THIS YOU RETARD
    //TODO :FIX THIS YOU RETARD
    //TODO :FIX THIS YOU RETARD
    //TODO :FIX THIS YOU RETARD
    //TODO :FIX THIS YOU RETARD
    //TODO :FIX THIS YOU RETARD 
    // TODO : the values are fliped for the array it goes y then x
    // probably should fix this
    // I think this is a bigger issue with the chunk or something 
    float** createNoise()
    {
        // large step few grid points, smooth
        // small step lots of grid points, bumpy
        const int randomSideLength = sideLength / step + 1;

        // another data structure might be better to use
        float randomNumberArray[randomSideLength][randomSideLength];

        // set random gen -> this can be improved
        // std::default_random_engine gen;
        // std::uniform_real_distribution<float> distribution(distroLow, distroHigh);  
        std::random_device rd;
        std::default_random_engine gen(rd());
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

                // need to fix this meshing together
                if(x > sideLength - 3 ||  y > sideLength - 3 || x < 3 || y  < 3)
                {
                    finilizedArray[y][x] = 0.0f;
                }
                else
                {
                    finilizedArray[y][x] = value;
                }
                
            }
        }  

        return finilizedArray;
    }

    // lerp - helper noise function
    float lerp(float a, float b, float t)
    {
        // apply smoothstep
        // 6x^5 - 15x^4 + 10x^3
        t = t * t * t * (t * (t * 6 - 15) + 10);

        return a + t * (b - a);
    }
};

#endif