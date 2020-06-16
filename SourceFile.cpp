#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class SourceFile
{
    //Read source file from disk
    public: char *readSourceFile(const char *filename, int *size)
    {
     
     //Open the file
     FILE *file;
     file = fopen(filename, "r");
     
     if(file == 0)
     {
         perror("fopen");
         exit(1);
     }
     
     //Find the end of the file to know the file size
     fseek(file, 0, SEEK_END);
     long fileSize = ftell(file);
     
     //Rewind
     fseek(file, 0, SEEK_SET);
     
     //Allocate memory for the source and initialize it to 0
     char *source = (char *)malloc(fileSize + 1);
     for (int i = 0; i <= fileSize; i++) source[i] = 0;
     
     //Read the source
     fread(source, fileSize, 1, file);
     
     //Close the file
     fclose(file);
     
     //Store the size of the file in the output variable
     *size = fileSize-1;
     
     //Return the shader source
     return source;
   }
    
};

