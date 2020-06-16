#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "SourceFile.cpp"

class Shader
{
    public: GLuint addVertexShader(const char* input, GLuint shaderID)
    {
        SourceFile file;
        shaderID = glCreateShader(GL_VERTEX_SHADER);
        int length = 0;
        char *source = file.readSourceFile(input, &length);
        glShaderSource(shaderID, 1, &source, &length);
        glCompileShader(shaderID);
          
        //Checks for compile time errors
        GLint success;
        GLchar infolog[512];
          
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
          
        if(!success)
        {
            glGetShaderInfoLog(shaderID, 512, NULL, infolog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
        }
          
        free(source);
        return shaderID;
    
    }
    public: GLuint addFragmentShader(const char* input,GLuint shaderID)
    {
           SourceFile file;
           shaderID = glCreateShader(GL_FRAGMENT_SHADER);
           int length = 0;
           char *source = file.readSourceFile(input, &length);
           glShaderSource(shaderID, 1, &source, &length);
           glCompileShader(shaderID);
             
           //Checks for compile time errors
           GLint success;
           GLchar infolog[512];
             
           glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
             
           if(!success)
           {
               glGetShaderInfoLog(shaderID, 512, NULL, infolog);
               std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
           }
             
           free(source);
           return shaderID;
    }
    
};
