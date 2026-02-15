#ifndef CAMERA_H
#define CAMERA_H

#include "../dependencies/glad.h"
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include <unordered_map>
#include <string>
#include <math.h>
#include <algorithm>

// TODO: make a barrior on camera movement, if player goes to far to stop them
// from exiting checking distance

enum Camera_Movement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    FPS,
    SPEED_INCREASE,
    SPEED_DECREASE

};

// default camera values
const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 90.0f;
const float OFFSET = 1024.0f;

class Camera
{
public:
    // camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    //euler angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // fps flagg
    bool fpsFlag = false;
    float biomeOffset= 1024.0f;

    // I need to mess around with offset
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), float offset = OFFSET, float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 1.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        biomeOffset = offset;
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float offset, float yaw, float pitch) : Front(glm::vec3(0.0f, 1.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        biomeOffset = offset;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, float** heightMap, float vertexSize = 0)
    {
        if(direction == SPEED_INCREASE)
            MovementSpeed += .25;
        if(direction == SPEED_DECREASE)
            MovementSpeed -= .25;

        float velocity = MovementSpeed * deltaTime;

        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
        if (direction == FPS)
            fpsFlag = !fpsFlag;

        // my coords are gonna gonna equal the nearest 

        // for both x and y look for nearest value to a vertex size
        // for instance x = 0.024
        // vertex number is 0.05 
        // we want to make x = 0
        if(fpsFlag)
        {
            int x = std::floor((Position.x / vertexSize));
            int y = std::floor((Position.y / vertexSize));

            if(x < 0) x = biomeOffset + x - 1;
            if(y < 0) y = biomeOffset + y - 1;

            try
            {
                //std::cout << heightMap[y][x] << '\n';
                Position.z = heightMap[y][x] + 2.0f;
            }
            catch (...) {
                std::cout << "value out of range" << '\n';
                Position.z =  2.0f;
            }
        }


    }
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   -= xoffset;
        Pitch += yoffset;

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {

        // to look towards +y axis
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y =  sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.z = sin(glm::radians(Pitch));
        Front = glm::normalize(front);     

        // Right handed coordinate system: +X to the right, +Z up
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));   
    }

    int wrapIndex(int value, int size)
    {
        value %= size;
        if (value < 0) value += size;
        return value;
    }
};


#endif