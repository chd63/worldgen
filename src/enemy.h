#ifndef ENEMY_H
#define ENEMY_H
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include <math.h>
#include <iostream>
#include "soundengine.h"

enum Level {
  LOW,
  MEDIUM,
  HIGH
};

class Enemy
{
public:
    glm::vec3 myPos = glm::vec3(0.0f,20.0f,0.0f);
    glm::vec3 attractorPos = glm::vec3(0.0f,0.0f,0.0f);
    float closeDis = 5;
    float mediumDis = 10;
    float farDis = 20;
    // ratio is m : n
    float m = 10;
    float n = 1;
    float distance;
    enum Level scareIndicator;

    // for sound
    SoundEngine* sound;
    SoundEngine* manic;
    SoundEngine* breath;
    bool jumpScareSound = true;

    Enemy()
    {
        sound = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/scare.mp3");
        manic = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/manic.mp3");
        breath = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/loadbreathing.mp3");
        
    }
    
    Enemy(glm::vec3 mypos)
    {
        myPos = mypos;
        sound = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/scare.mp3");
        manic = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/manic.mp3");
        breath = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/loadbreathing.mp3");
    }

    Enemy(glm::vec3 mypos, glm::vec3 otherpos)
    {
        myPos = mypos;
        attractorPos = otherpos;
        sound = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/scare.mp3");
        manic = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/manic.mp3");
        breath = new SoundEngine("/home/dev/projects_for_fun/opengl_projects/worldgen/sounds/loadbreathing.mp3");
    }

    void updateMyPos(glm::vec3 pos)
    {
        myPos = pos;
    }

    bool updateEnemyPos(glm::vec3 pos)
    {
        attractorPos = pos;

        // TODO: each distance should affect a random probablity that
        // will effect the ratio, this will be added later as it adds more
        // complexity

        // distance - calculate distance formula
        distance = sqrt( ((myPos.x - attractorPos.x) * (myPos.x - attractorPos.x) )  
            + ((myPos.y - attractorPos.y) * (myPos.y - attractorPos.y) )  );

        //std::cout << distance << "\n";

        // if distance is within closeDis
        if(distance <= closeDis)
        {
            if(jumpScareSound)
            {
                sound->playSound();
                jumpScareSound = false;
            }
            return true;
        } 

        // if distance within mediumDis
        if(distance <= mediumDis)
        {
            if(scareIndicator != HIGH)
            {
                manic->playSound();
                scareIndicator = HIGH;
            }
            m = 12;
        }
        else if(distance <= farDis)
        {
            if(scareIndicator != MEDIUM)
            {
                breath->playSound();
                scareIndicator = MEDIUM;
            }
            m = 10;
        }
        else
        {
            if(scareIndicator != LOW)
            {

                scareIndicator = LOW;
            }
            m = 5;
        }

        // calculate new x value
        // Section Formula
        myPos.x = ( (m * myPos.x ) + ( n * attractorPos.x ) ) / ( m + n ) ;

        //std::cout << "Pos x " << myPos.x  << "\n";

        // calculate new y value
        myPos.y = ( (m * myPos.y ) + ( n * attractorPos.y ) ) / ( m + n ) ;

        //std::cout << "Pos y " << myPos.y  << "\n";

        return false;
    }

    ~Enemy()
    {
        delete sound;
        delete manic;
        delete breath;
    }

private:

};

#endif