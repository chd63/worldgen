#ifndef WORLDGEN_H
#define WORLDGEN_H
#include "biome.h"
#include "chunk.h"

class World
{
public:
    // variables & data
    const int step = 8;
    const int distroLow = -10.0f;
    const int distroHigh = 20.0f;

    // render distance
    unsigned int renderDistance = 4;
    float numberOfBiomes = 4;
    float biomeSideLength = 512;
    std::unordered_map<std::string, Biome> biomes;

    glm::vec3 playerPosition = glm::vec3(0.0f,0.0f,0.0f);

    float chunkSize = 32.0f;
    float triangleSize = 1.0f;
    int numberOfChunks = 9;
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

    // update chunck info
    void updateChunks()
    {
        // we need to use player position and number of bioms to determine 
        chunks.clear();
        chunks.shrink_to_fit();
        chunks.reserve(numberOfChunks);
        
        // we will have a center chunk where we will check the values of the player position
        float originX = ( std::floor(playerPosition.x / chunkSize) * chunkSize) - chunkSize ;
        float originY = (std::floor(playerPosition.y / chunkSize) * chunkSize) - chunkSize;

        // get the biome
        float bx = ( std::floor(originX / biomeSideLength) * biomeSideLength);
        float by = ( std::floor(originY / biomeSideLength) * biomeSideLength);
        std::string key= std::to_string(bx) + " " + std::to_string(by);
        //std::cout << key << '\n';
        //std::cout << biomes[key].biomeData[15][25] << '\n';

        int sideLength =int(std::sqrt(numberOfChunks));
        int sideValue = 0;
        for(int x = 0; x < numberOfChunks; x++ )
        {
            //std::cout << originX << " " << originY << '\n';
            // get the y value of the specific input
            chunks.push_back(std::make_unique<Chunk>( triangleSize, chunkSize,glm::vec3(originX,originY, 0.0f ), biomes[key].biomeData));
            

            if(sideValue == sideLength - 1)
            {
                sideValue = 0;
                originY += chunkSize;
                originX = ( std::floor(playerPosition.x / chunkSize) * chunkSize) - chunkSize;
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

private:


};


#endif