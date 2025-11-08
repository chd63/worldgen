#include "../dependencies/glad.h"
#include <GLFW/glfw3.h>
#include "../dependencies/shader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"
#include "../dependencies/glm/gtc/type_ptr.hpp"
#include "camera.h"
#include "worldgen.h"
#include "text.h"
#include <ft2build.h>
#include FT_FREETYPE_H  


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, std::unordered_map<std::string ,float>& heightMap);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 960;

// camera
Camera camera(glm::vec3(0.0f,0.0f,0.5f),glm::vec3(0.0f, 0.0f, 1.0f));  
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// this is for frame counter
float elapsed = 0.0f;
int frame_tracker = 0;
bool framesOn = true;
bool axisOn = false;
std::string frameCountText = "DEFAULT";
const float STANDARD_PLANE_LENGTH = 250.0;
const float TRIANGLE_LENGTH = 1.0;

int main()
{
    // load fonts 
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return -1;
    }


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // callbacks
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetScrollCallback(window, scroll_callback); 
    
    // tell GLFW to capture out mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // config gloabal opengl state
    glEnable(GL_DEPTH_TEST);  

    // build and compile our shader program
    // ------------------------------------
    // use reletive file for rn
    Shader ourShader("/home/dev/projects_for_fun/opengl_projects/worldgen/shaders/shader_1.vss",
         "/home/dev/projects_for_fun/opengl_projects/worldgen/shaders/shader_1.fss"); // you can name your shader files however you like

    Shader axisShader("/home/dev/projects_for_fun/opengl_projects/worldgen/shaders/axisShader.vss",
         "/home/dev/projects_for_fun/opengl_projects/worldgen/shaders/axisShader.fss");

    Shader textShader("/home/dev/projects_for_fun/opengl_projects/worldgen/shaders/textShader.vss",
         "/home/dev/projects_for_fun/opengl_projects/worldgen/shaders/textShader.fss");


    Plane plane = createPlaneThroughTriangles(TRIANGLE_LENGTH, STANDARD_PLANE_LENGTH);
    float *vertices= plane.vertices;
    unsigned int *indices= plane.indices;

    // for (const auto& pair : plane.heightMap) {
    //     std::cout << pair.first << " : " << pair.second << '\n';
    // }

    glm::vec3 planePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3(-250.0f, 0.0f, 0.0f),
        glm::vec3(-250.0f, -250.0f, 0.0f),
        glm::vec3(0.0f, -250.0f, 0.0f)
        
    };


    // x y z
    // create a vector of of lines to demontrate the axis
    float axisVerticies[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, -3.0f, 1.0f, 1.0f, 0.0f

    };

    unsigned int axisIndices[] = {
        0, 1 , 0 , 2 , 0, 2, 0, 3, 0, 4, 0, 5, 0, 6
    };

    unsigned int VBO1, VAO1, EBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &EBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, 42 * sizeof(float), axisVerticies, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 14 * sizeof(unsigned int), axisIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, plane.vertex_length * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.indices_length * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TODO: implement textures
    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("/home/dev/projects_for_fun/opengl_projects/worldgen/textures/brick.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    FT_Set_Pixel_Sizes(face, 0, 48);  

    std::map<char, Character> Characters;

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // enable belending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    // get space for the text objects
    unsigned int VAO3, VBO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);  

    // uncomment this call to draw in wireframe polygons. 1/2
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    ourShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, plane.heightMap);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader use
        ourShader.use();

        ourShader.setInt("texture1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transfomration
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view",view);


        // render our plane
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 4; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, planePositions[i]);
            ourShader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, plane.indices_length , GL_UNSIGNED_INT,0);
        }

        if(axisOn)
        {
            // dont need to draw two shaders, but doing this for practice
            axisShader.use();
            axisShader.setMat4("projection", projection);
            axisShader.setMat4("view",view);
            // render axis
            glBindVertexArray(VAO1);
            glLineWidth(5.0f);
            axisShader.setMat4("model", glm::mat4(1.0f));
            glDrawElements(GL_LINES, sizeof(axisIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
            // reset to default
            glLineWidth(1.0f); 

            RenderText(textShader, Characters, VAO3, VBO3, "BLUE = + Z Axis",
                550.0f, 50.0f, 0.4f, glm::vec3(0.0f, 0.0f, 1.0f));
            RenderText(textShader, Characters, VAO3, VBO3, "YELLOW = - Z Axis",
                550.0f, 75.0f, 0.4f, glm::vec3(1.0f, 1.0f, 0.0f));
               
            RenderText(textShader, Characters, VAO3, VBO3, "LIGHT BLUE = + Y Axis",
                550.0f, 125.0f, 0.4f, glm::vec3(0.0f, 1.0f, 1.0f));
            RenderText(textShader, Characters, VAO3, VBO3, "GREEN = - Y Axis",
                550.0f, 100.0f, 0.4f, glm::vec3(0.0f, 1.0f, 0.0f));
             

            RenderText(textShader, Characters, VAO3, VBO3, "RED = + X Axis",
                550.0f, 175.0f, 0.4f, glm::vec3(1.0f, 0.0f, 0.0f));
            RenderText(textShader, Characters, VAO3, VBO3, "PURPLE = - X Axis",
                550.0f, 150.0f, 0.4f, glm::vec3(1.0f, 0.0f, 1.0f));
             
        }
        
        // per fram time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        if(framesOn == true){
            elapsed += deltaTime;
            frame_tracker++;
            
            if (elapsed >= 1.0f){
                frameCountText = std::to_string(frame_tracker);
                //std::cout << frame_tracker << "\n";

                frame_tracker = 0;
                elapsed = 0.0f;
            }
            RenderText(textShader, Characters, VAO3, VBO3, frameCountText,
                     25.0f, 25.0f, 0.8f , glm::vec3(0.5, 0.8f, 0.2f));            
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // TODO: MAKE A BETTER APPROCH
    delete[] plane.vertices;
    delete[] plane.indices;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, std::unordered_map<std::string ,float>& heightMap)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // movementf
    const float cameraSpeed = 2.5f * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime, heightMap, TRIANGLE_LENGTH);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime, heightMap, TRIANGLE_LENGTH);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime, heightMap, TRIANGLE_LENGTH);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime, heightMap, TRIANGLE_LENGTH);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime, heightMap, TRIANGLE_LENGTH);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime, heightMap, TRIANGLE_LENGTH);

    // TODO: figure out how to give this some more time so you dont have play a fucking timing game
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS)
        axisOn = !axisOn;
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS)
        framesOn = !framesOn;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS)
        camera.ProcessKeyboard(FPS, deltaTime, heightMap, TRIANGLE_LENGTH);
    if (glfwGetKey(window, GLFW_KEY_EQUAL ) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS)
        camera.ProcessKeyboard(SPEED_INCREASE, deltaTime, heightMap, TRIANGLE_LENGTH);
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS)
        camera.ProcessKeyboard(SPEED_DECREASE, deltaTime, heightMap, TRIANGLE_LENGTH);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

