#ifndef WORLDGEN_H
#define WORLDGEN_H
#include "biome.h"
#include "chunk.h"

class World
{
public:
    // variables & data
    const int step = 32;
    const int distroLow = -10.0f;
    const int distroHigh = 40.0f;

    // render distance
    unsigned int renderDistance = 4;
    float numberOfBiomes = 4;
    float biomeSideLength = 1024;
    std::unordered_map<std::string, Biome> biomes;

    glm::vec3 playerPosition = glm::vec3(0.0f,0.0f,0.0f);

    float chunkSize = 32.0f;
    float triangleSize = 1.0f; // TODO: make a constructor to change this
    int numberOfChunks = 25;
    std::vector<std::unique_ptr<Chunk>> chunks;

    // constructor
    World()
    {
        // calculate start this will be the most bottom left value
        float start = ( (numberOfBiomes / 4) * biomeSideLength ) * -1;
        float end  = (numberOfBiomes / 4) * biomeSideLength ;

        //std::cout << start << " " << end << '\n';

        for(float x = start; x < end; x += biomeSideLength)
        {
            for(float y = start; y < end; y += biomeSideLength)
            {
                // generate a biome
                Biome tempBiome(biomeSideLength, step , distroLow , distroHigh);

                std::string key= std::to_string(x) + " " + std::to_string(y);

                //std::cout << key << '\n';
                // insert biome into hashmap at location
                biomes[key] = tempBiome;

            }
        }
        //std::cout << biomes["0.000000 0.000000"].biomeData[10][10] << '\n';

    }     

    // update player position
    void updatePlayerPosition(glm::vec3 newPlayerPos)
    {
        playerPosition = newPlayerPos;
    }

    //TODO :update this after fixing biomes
    // if you have a world position at like anything not postive it breaks
    // rn it works with 50 x 50 y but if you go smaller it tweaks

    // update chunck info
    void updateChunks()
    {
        // we need to use player position and number of bioms to determine 
        chunks.clear();
        chunks.shrink_to_fit();
        chunks.reserve(numberOfChunks);
        
        // we will have a center chunk where we will check the values of the player position
        float originX = (std::floor(playerPosition.x  / chunkSize)* chunkSize) - (((std::sqrt(numberOfChunks) - 1 ) / 2) * chunkSize);
        float originY = (std::floor(playerPosition.y  / chunkSize)* chunkSize) - (((std::sqrt(numberOfChunks) - 1 ) / 2) * chunkSize);

        int sideLength =int(std::sqrt(numberOfChunks));
        int sideValue = 0;
        for(int x = 0; x < numberOfChunks; x++ )
        {
            // get the biome
            float bx = ( std::floor(originX / biomeSideLength) * biomeSideLength);
            float by = ( std::floor(originY / biomeSideLength) * biomeSideLength);
            std::string key= std::to_string(bx) + " " + std::to_string(by);
            //std::cout << key << '\n';
            //std::cout << biomes[key].biomeData[15][25] << '\n';

            // get the y value of the specific input
            // std::cout << originX << " " << originY << '\n';
            chunks.push_back(std::make_unique<Chunk>( triangleSize, chunkSize,glm::vec3(originX,originY, 0.0f ), biomes[key].biomeData));

            if(sideValue == sideLength - 1)
            {
                sideValue = 0;
                originY += chunkSize;
                originX = (std::floor(playerPosition.x  / chunkSize)* chunkSize) - (((std::sqrt(numberOfChunks) - 1 ) / 2) * chunkSize);
                bx = ( std::floor(originX / biomeSideLength) * biomeSideLength);
                by = ( std::floor(originY / biomeSideLength) * biomeSideLength);

            }
            else
            {
                originX += chunkSize;
                bx = ( std::floor(originX / biomeSideLength) * biomeSideLength);
                sideValue++;
            }
            key= std::to_string(bx) + " " + std::to_string(by);
            //std::cout << key << '\n';
            
        }

        //std::cout << chunks[0]->localOrigin.x << '\n';
    }

    Biome returnCurrentBiomeArrayByPosition(glm::vec3 playerPos)
    {
        float x = std::floor((playerPos.x / biomeSideLength)) * biomeSideLength;
        float y = std::floor((playerPos.y / biomeSideLength)) * biomeSideLength;
        std::string temp = std::to_string(y) + " " + std::to_string(x) ;
        //std::cout << temp << '\n';
        return biomes[temp];
    }

private:


};


#endif