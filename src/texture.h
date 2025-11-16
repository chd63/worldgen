#ifndef TEXTURE_H
#define TEXTURE_H


class Texture{
public:
    int width, height, nrChannels;
    unsigned int ID;

    Texture(const char* textureLocation)
    {
        stbi_set_flip_vertically_on_load(true);

        unsigned char* data = stbi_load(textureLocation, &width, &height, &nrChannels, 0);
        if (data)
        {
            createBuffer();
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
            ID = 0;
        }
        stbi_image_free(data);
    }

    void bind(GLenum textureUnit = GL_TEXTURE0)
    {
        if (ID == 0)
            return; // prevent binding invalid texture
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }


private:
    void createBuffer()
    {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

};

#endif