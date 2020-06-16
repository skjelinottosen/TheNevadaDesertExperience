#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "resources/stb_image/stb_image.h"

class Texture
{
     public: static GLuint LoadTexture(GLchar *path, GLuint textureID)
     {
    
        GLint width, height, nrChannels;
        
        //Texture
        //Generate texture name
        glGenTextures(1, &textureID);
           
        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set the tedture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
           
        // Load image from file
        
        GLubyte *imageData = stbi_load(path, &width, &height, &nrChannels, 3);
        if (imageData)
        {
            // Specify the format of the texture
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Not found! Failed to load texture." << std::endl;
        }
           
        stbi_image_free(imageData);
           
        //Deactivate texture
        glBindTexture(GL_TEXTURE_2D, 0);
        
        return textureID;
    }
    
    public: static GLuint LoadCubeMap(std::vector<const GLchar *> sides,  GLuint textureID)
    {
        GLint width, height, nrChannels;
        GLubyte *imageData;
           
        //Texture
        //Generate texture name
        glGenTextures(1, &textureID);
              
        //Bind the texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        //Set the texture parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        //Set texture filtering
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
              
        //Load images from file
        for (GLuint i = 0; i < sides.size(); i++)
        {
            imageData = stbi_load(sides[i], &width, &height, &nrChannels, 3);
            if (imageData)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            }
            
            else
            {
                std::cout << "Not found! Failed to load lanscape texture." << std::endl;
            }
              
            //Specify the format of the texture
            glTexImage2D(GL_TEXTURE_CUBE_MAP, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
            stbi_image_free(imageData);
        }

        //Deactivate texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        return textureID;
    }
};


