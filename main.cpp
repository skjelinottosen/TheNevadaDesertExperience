#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "shader.cpp"
#include "Texture.cpp"
#include "Camera.h"
//Sound library
#include "resources/irrklang/include/irrKlang.h"
#include "resources/conio.h"
//Link with irrKlang.dll
#pragma comment(lib, "irrKlang.lib")

#define TINYOBJLOADER_IMPLEMENTATION
#include "resources/tiny_obj_loader.h"

//Window size
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080

//Vertex Buffer Identifiers
#define VERTICES 0
#define INDICES 1


//Vertex Array attributes
#define POSITION 0
#define NORMAL 1
#define UV 2

using namespace irrklang;

//Names for storing mesh data
typedef struct
{
    GLuint bufferNameObject;
    GLuint arrayNameObject;
    GLuint textureObject;
    GLsizei numVerticesObject;
} Mesh;

//Vector for storing mashes
std::vector<Mesh> meshes;

unsigned int landscapeVAO, landscapeVBO;

//Function prototype
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
int loadObj(const char *objectName);


//Vertices
GLfloat cube[] =
{
    //Position 1,2,3   Texture 4,5   Normal 6,7,8
    // Front
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    // Back
    1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    // Left
    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    // Right
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    // Top
    -1.0f, 1.0f, -1.0f,0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    // Bottom
    -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f
};

//Vertices
float landscape[] =
{
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

//Indices
GLushort indicesCube[] =
{
    //Front
    0, 1, 2, 2, 3, 0,
    //Back
    4, 5, 6, 6, 7, 4,
    //Left
    8, 9, 10, 10, 11, 8,
    //Right
    12, 13, 14, 14, 15, 12,
    //Top
    16, 17, 18, 18, 19, 16,
    //Bottom
    20, 21, 22, 22, 23, 20
};

//Position for drawing mutiple cubes
glm::vec3 positionsCube[] =
{
    //Golde cube
    glm::vec3( 30.0f,  0.0f,  0.0f),
    //Reflection cube
    glm::vec3( 0.0f,  -5.0f,  0.0f),
    //Newspaper
    glm::vec3( -50.0f,  -20.0f,  50.0f),
    
};

//Uniforms values
GLfloat lightAmbient[] = {  0.4f, 0.4f, 0.4f };
GLfloat lightDiffuse[] = { 1.5f, 1.5f, 1.5f };
GLfloat lightSpecular[] = { 0.8f, 0.8f, 0.8f };
GLfloat materialShininessColor[] = { 0.6f, 0.6f, 0.6f,  1.0f };
GLfloat materialShininess = 22.0f;
GLfloat lightDirection[] = {30.0f, 0.0f, 3.0f, 0.0f };
GLfloat cameraPosition[] = { -24.0f, -8.0f, 40.0f };

//Program names
GLuint cubeProgram;
GLuint reflectCubeProgram;
GLuint landscapeProgram;
//Vertcties and buffer names
GLuint vertexArrayName;
GLuint vertexBufferNames[2];
GLuint vertexArrayName2;
GLuint vertexBufferNames2[2];

//Texture names
GLuint landscapeTexture;
GLuint landscapeReflectionTexture;
GLuint textureBackgroundSand;
GLuint normalMapGold;
GLuint newspaperTexture;
GLuint normalMapWhite;

//Uniform locations
GLint projectionMatrixPos;
GLint viewMatrixPos;
GLint modelMatrixPos;
GLint lightPositionPos;
GLint lightAmbientPos;
GLint lightDiffusePos;
GLint lightSpecularPos;
GLint materialShininessColorPos;
GLint materialShininessPos;
GLint cameraPositionPos;
GLuint lightDirectionPos;
GLint reflectProjectionMatrixPos;
GLint reflectViewMatrixPos;
GLint reflectModelMatrixPos;
GLint reflectLightPositionPos;
GLint reflectLightAmbientPos;
GLint reflectLightDiffusePos;
GLint reflectLightSpecularPos;
GLint reflectMaterialShininessColorPos;
GLint reflectMaterialShininessPos;
GLint reflectCameraPositionPos;
GLuint reflectLightDirectionPos;


/*-Initialize OpenGL-*/
int initGL()
{
/*----------------- Buffer 1 -----------------------------*/
    //Create buffer names for cube and cube indicies
    glGenBuffers(2, vertexBufferNames);
    
    //Allocate storage for the cube
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNames[VERTICES]); // 2.0
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * 8 * sizeof(GLfloat), cube, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //Allocate storage for the cube indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferNames[INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * 6 * sizeof(GLushort),indicesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //Create the vertex array object
    glGenVertexArrays(1, &vertexArrayName); // 3.0
    
    //Initialize the vertex array object
    glBindVertexArray(vertexArrayName); // 3.0
    
    //Specify attribute format for the cube
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNames[VERTICES]);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0); // 3.0
    glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT)));
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (const void *)(3 * sizeof(GL_FLOAT)));
    
    //Enable the attributes for the cube
    glEnableVertexAttribArray(POSITION); // 2.0
    glEnableVertexAttribArray(UV);
    glEnableVertexAttribArray(NORMAL);
    
    //Disable the vertex array for the cube
    glBindVertexArray(0);
    
    
/*------------- Buffer 2 -----------------------------*/
    
    //Create buffer names for cube and cube indicies
    glGenBuffers(2, vertexBufferNames2);
    
    //Allocate storage for the cube
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNames2[VERTICES]); // 2.0
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * 8 * sizeof(GLfloat), cube, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //Allocate storage for the cube indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferNames2[INDICES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * 6 * sizeof(GLushort), indicesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //Create the vertex array object
    glGenVertexArrays(1, &vertexArrayName2); // 3.0
    //Initialize the vertex array object
    glBindVertexArray(vertexArrayName2); // 3.0
    
    //Specify attribute format for the cube
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNames2[VERTICES]);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0); // 3.0
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (const void *)(3 * sizeof(GL_FLOAT)));
    
    //Enable the attributes for the cube
    glEnableVertexAttribArray(POSITION); // 2.0
    glEnableVertexAttribArray(NORMAL);
    
    //Disable the vertex array for the cube
    glBindVertexArray(0);
    
    //Create the vertex array objects
    glGenVertexArrays(1, &landscapeVAO);
    glGenBuffers(1, &landscapeVBO);
    
    //Initialize the vertex array objects
    glBindVertexArray(landscapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, landscapeVBO);
    
    //Allocate storage for the landscape
    glBufferData(GL_ARRAY_BUFFER, sizeof(landscape), &landscape, GL_STATIC_DRAW);
    
    //Specify attribute format for the cube
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //Enable the attributes for the landscape
    glEnableVertexAttribArray(POSITION);
    
    glBindVertexArray(0);
    //Sets the dep function back to default
    glDepthFunc(GL_LESS);
    
    //Loading the cube texture
    textureBackgroundSand = Texture::LoadTexture("resources/images/sand.jpg",0);
    normalMapGold  = Texture::LoadTexture("resources/images/illuminati_normal.jpg", 1);
    newspaperTexture = Texture::LoadTexture("resources/images/rosewell.jpg", 2);
    
    //Inserts elements into the vector
    std::vector<const GLchar *> sides;
    {
        sides.push_back("resources/images/landscape/right.jpg");
        sides.push_back("resources/images/landscape/left.jpg");
        sides.push_back("resources/images/landscape/top.jpg");
        sides.push_back("resources/images/landscape/bottom.jpg");
        sides.push_back("resources/images/landscape/back.jpg");
        sides.push_back("resources/images/landscape/front.jpg");
    };
    
    //Loading the texture for the landscape
    landscapeTexture = Texture::LoadCubeMap(sides, 0);
    
    //Inserts elements into the vector
    std::vector<const GLchar *> sidesReflection;
    {
        sidesReflection.push_back("resources/images/landscape/right.jpg");
        sidesReflection.push_back("resources/images/landscape/leftAlien.jpg");
        sidesReflection.push_back("resources/images/landscape/top.jpg");
        sidesReflection.push_back("resources/images/landscape/bottom.jpg");
        sidesReflection.push_back("resources/images/landscape/backAlien.jpg");
        sidesReflection.push_back("resources/images/landscape/front.jpg");
    };
    
    //Loading the texture for the relection of the landscape
    landscapeReflectionTexture  = Texture::LoadCubeMap(sidesReflection, 1);
    
    //Prevent the backside of the cube to render over elements in front section of the cube
    glEnable(GL_DEPTH_TEST);
    
    //Loading shaders
    Shader vertexShader, fragmentShader, landscapeVertexShader, landscapeFragmentShader, reflectionVertexShader,reflectionFragmentShader;
    GLuint vertexShaderName = vertexShader.addVertexShader("resources/shaders/cube.vert", 0);
    GLuint fragmentShaderName = fragmentShader.addFragmentShader("resources/shaders/cube.frag", 1);
    GLuint landscapeVertexShaderName = landscapeVertexShader.addVertexShader("resources/shaders/landscape.vert", 2);
    GLuint landscapeFragmentShaderName = landscapeFragmentShader.addFragmentShader("resources/shaders/landscape.frag", 3);
    GLint reflectionVertexShaderName = reflectionVertexShader.addVertexShader("resources/shaders/reflectCube.vert", 4);
    GLint reflectionFragmentShaderName = reflectionFragmentShader.addFragmentShader("resources/shaders/reflectCube.frag", 5);
    
    //Create vertex programs for the cube
    cubeProgram = glCreateProgram();
    glAttachShader(cubeProgram, vertexShaderName);
    glAttachShader(cubeProgram, fragmentShaderName);
    
    //Links the vertex programs for the cube
    glLinkProgram(cubeProgram);
    
    //Error handling
    GLint linkStatus;
    glGetProgramiv( cubeProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        GLint logSize = 0;
        glGetProgramiv( cubeProgram, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *) malloc(sizeof(char) * logSize);
        glGetProgramInfoLog( cubeProgram, logSize, &logSize, errorLog);
        
        printf("LINK ERROR CUBE %s\n", errorLog);
        return 0;
    }
    
    //Create vertex programs for the landscape
    landscapeProgram = glCreateProgram();
    glAttachShader(landscapeProgram, landscapeVertexShaderName);
    glAttachShader(landscapeProgram, landscapeFragmentShaderName);
    
    //Links vertex programs for the cube
    glLinkProgram(landscapeProgram);
    
    //Error handling
    glGetProgramiv(landscapeProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        GLint logSize = 0;
        glGetProgramiv(landscapeProgram, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *) malloc(sizeof(char) * logSize);
        glGetProgramInfoLog(landscapeProgram, logSize, &logSize, errorLog);
        
        printf("LINK ERROR LANDSCAPE %s\n", errorLog);
        return 0;
    }
    
    //Create vertex programs for the reflection cube
    reflectCubeProgram = glCreateProgram();
    glAttachShader(reflectCubeProgram, reflectionVertexShaderName);
    glAttachShader(reflectCubeProgram, reflectionFragmentShaderName);
    
    //Links vertex programs for the cube
    glLinkProgram(reflectCubeProgram);
    
    //Error handling
    glGetProgramiv(reflectCubeProgram, GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        GLint logSize = 0;
        glGetProgramiv(reflectCubeProgram, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *) malloc(sizeof(char) * logSize);
        glGetProgramInfoLog(reflectCubeProgram, logSize, &logSize, errorLog);
        
        printf("LINK ERROR REFLECTION CUBE PROGRAM %s\n", errorLog);
        return 0;
    }
    
    //Get the locations of the uniform variables in the program object
    projectionMatrixPos = glGetUniformLocation( cubeProgram, "proj");
    viewMatrixPos = glGetUniformLocation( cubeProgram, "view");
    modelMatrixPos = glGetUniformLocation( cubeProgram, "model");
    lightPositionPos = glGetUniformLocation( cubeProgram, "lightPosition");
    lightAmbientPos = glGetUniformLocation( cubeProgram, "lightAmbient");
    lightDiffusePos = glGetUniformLocation( cubeProgram, "lightDiffuse");
    lightSpecularPos = glGetUniformLocation( cubeProgram, "lightSpecular");
    materialShininessColorPos = glGetUniformLocation( cubeProgram, "shininessColor");
    materialShininessPos = glGetUniformLocation( cubeProgram, "shininess");
    cameraPositionPos = glGetUniformLocation( cubeProgram, "cameraPosition");
    lightDirectionPos = glGetUniformLocation( cubeProgram, "lightDirection");
    
    //Get the locations of the uniform variables in the program object
    reflectProjectionMatrixPos = glGetUniformLocation( reflectCubeProgram, "proj");
    reflectViewMatrixPos = glGetUniformLocation( reflectCubeProgram, "view");
    reflectModelMatrixPos= glGetUniformLocation( reflectCubeProgram, "reflectModel");
    reflectLightPositionPos = glGetUniformLocation( reflectCubeProgram, "lightPosition");
    reflectLightAmbientPos = glGetUniformLocation( reflectCubeProgram, "lightAmbient");
    reflectLightDiffusePos = glGetUniformLocation( reflectCubeProgram, "lightDiffuse");
    reflectLightSpecularPos = glGetUniformLocation( reflectCubeProgram, "lightSpecular");
    reflectMaterialShininessColorPos = glGetUniformLocation(reflectCubeProgram, "shininessColor");
    reflectMaterialShininessPos = glGetUniformLocation( reflectCubeProgram, "shininess");
    reflectCameraPositionPos = glGetUniformLocation( reflectCubeProgram, "cameraPosition");
    reflectLightDirectionPos = glGetUniformLocation(reflectCubeProgram, "lightDirection");
    
    return 1;
    
}

/* Draw OpenGL screne */
void drawGLScene() {
    
    //Clear color and depth buffers
    glClearColor(0.9, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Initialize identity matrix
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    
    
/*----------------Golden cube---------------------------------------------------------------------------*/
    
    //Activate the program for the cube
    glUseProgram(cubeProgram);
    //Set the view matrix
    view = glm::translate(view, glm::vec3(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]));
    glUniformMatrix4fv(viewMatrixPos, 1, GL_FALSE, &view[0][0]); // 2.0
    
    //Set the model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, positionsCube[0]);
    float angle = 20.0f ;
    model = glm::scale(model, glm::vec3(4.0f, 4.0f,  4.0f));
    model = glm::rotate(model, (float)glfwGetTime() * (float)(0.7), glm::vec3(0.0f, -1.0f,  0.0f));
    glUniformMatrix4fv(modelMatrixPos, 1, GL_FALSE, &model[0][0]);
    
    //Set the moving light matrix
    glm::vec4 lightPos = glm::make_vec4(lightDirection);
    glm::mat4 movingLight = glm::mat4(1.0);
    lightPos = movingLight* lightPos;
    
    //Set the rest of the uniforms
    glUniform3f(lightAmbientPos, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
    glUniform3fv(lightDiffusePos, 1, lightDiffuse);
    glUniform3fv(lightSpecularPos, 1, lightSpecular);
    glUniform4fv(materialShininessColorPos, 1, materialShininessColor);
    glUniform1f(materialShininessPos, materialShininess);
    glUniform3fv(cameraPositionPos, 1, cameraPosition);
    glUniform3f(lightDirectionPos,lightPos.x, lightPos.y, lightPos.z);
    
    //Activate the cube texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureBackgroundSand);
    glUniform1i(glGetUniformLocation( cubeProgram, "diffuseMap"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMapGold);
    glUniform1i(glGetUniformLocation( cubeProgram, "normalMap"), 1);
    
    //Activate the vertex array
    glBindVertexArray(vertexArrayName);
    
    //Activate the cube indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferNames[INDICES]);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    
    //Disable
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    
/*----------------Newspaper---------------------------------------------------------------------------*/
    
    //Activate the program for the newspaper
    glUseProgram(cubeProgram);
    
    //Activate the newspaper texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, newspaperTexture);
    glUniform1i(glGetUniformLocation( cubeProgram, "diffuseMap"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, newspaperTexture);
    glUniform1i(glGetUniformLocation( cubeProgram, "normalMap"), 1);
    
    //Activate the vertex array
    glBindVertexArray(vertexArrayName);
    
    glUniformMatrix4fv(viewMatrixPos, 1, GL_FALSE, &view[0][0]); // 2.0
    glm::mat4 newsPaperModel = glm::mat4(1.0f);
    newsPaperModel = glm::translate(newsPaperModel, positionsCube[2]);
    newsPaperModel = glm::scale(newsPaperModel, glm::vec3(30.0f, 30.0f, 30.0f));
    newsPaperModel = glm::rotate(newsPaperModel,(float)(1.5708), glm::vec3(1.0f, 0.0f,  0.0f));
    glUniformMatrix4fv(modelMatrixPos, 1, GL_FALSE, &newsPaperModel[0][0]);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    
    //Disable
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
/*----------------Reflec cube---------------------------------------------------------------------------*/
   
    //Activate the program for the cube
    glUseProgram(reflectCubeProgram);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, landscapeReflectionTexture);
    glUniform1i(glGetUniformLocation( reflectCubeProgram, "landscape"), 0);
    
    //Activate the vertex array
    glBindVertexArray(vertexArrayName2);
    glUniformMatrix4fv(reflectViewMatrixPos, 1, GL_FALSE, &view[0][0]); // 2.0
    
    glm::mat4 reflectModel = glm::mat4(1.0f);
    reflectModel = glm::translate(reflectModel, positionsCube[1]);
    reflectModel = glm::scale(reflectModel, glm::vec3(10.0f,10.0f, 10.0f));
    reflectModel  = glm::rotate(reflectModel, (float)glfwGetTime()*(float)0.2, glm::vec3(0.0f, -1.0f,  0.0f));
    glUniformMatrix4fv( reflectModelMatrixPos, 1, GL_FALSE, &reflectModel[0][0]);
    
    //Activate the cube indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferNames2[INDICES]);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        
    //Disable
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    //Show both
    glDepthFunc(GL_LEQUAL);
    
    //Landscape
    glUseProgram(landscapeProgram);
    glm::mat4 landscapeView = glm:: mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
    glm::mat4 landscapeProj  = glm::perspective(3.14f/2.0f, (float)DEFAULT_WIDTH/DEFAULT_HEIGHT, 0.1f, 1000.0f);
    glUniformMatrix4fv( glGetUniformLocation(landscapeProgram, "landscapeView"), 1 , GL_FALSE, glm::value_ptr(landscapeView)); // 2.0
    glUniformMatrix4fv( glGetUniformLocation(landscapeProgram, "landscapeProj"), 1 , GL_FALSE, glm::value_ptr(landscapeProj)); // 2.0
    
    glBindVertexArray(landscapeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, landscapeTexture);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    //Set depth function back to default
    glDepthFunc(GL_LESS);
    
    //Disable
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    
/*----------------Flying UFO near---------------------------------------------------------------------*/
   
    //Activate the program for the UFO
    glUseProgram(cubeProgram);
    
    //Set the model matrix
    glm::mat4 ufoModel= glm::mat4(1.0);
    ufoModel = glm::translate(ufoModel, glm::vec3(23000.0f, -310.0f, -4000.0f));
    ufoModel = glm::scale(ufoModel, glm::vec3(0.3f, 0.3f, 0.3f));
    ufoModel = glm::rotate(ufoModel, (float)(1.5708), glm::vec3(1.0f, 0.0f,  -0.05f));
    ufoModel = glm::translate(ufoModel, glm::vec3((float)glfwGetTime() * -6000.0f, 40.0f, -1000.0f));
    glUniformMatrix4fv(modelMatrixPos, 1, GL_FALSE, &ufoModel[0][0]);
    
    
    //Set the remaining uniforms
    glUniform3fv(lightPositionPos, 1, lightDirection);
    glUniform3f(lightAmbientPos, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
    glUniform3fv(lightDiffusePos, 1, lightDiffuse);
    glUniform3fv(lightSpecularPos, 1, lightSpecular);
    glUniform4fv(materialShininessColorPos, 1, materialShininessColor);
    glUniform1f(materialShininessPos, materialShininess);
    glUniform3fv(cameraPositionPos, 1, cameraPosition);
    // Loop through all the meshes loaded from the OBJ-file
    for (int i=0; i<meshes.size(); ++i)
    {

         // Bind the vertex array and texture of the mesh
         glBindVertexArray(meshes[i].arrayNameObject);
         glBindTexture(GL_TEXTURE_2D, meshes[i].textureObject);

         // Draw the vertex array
         glDrawArrays(GL_TRIANGLES, 0, meshes[i].numVerticesObject);

         // Disable vertex array and texture
         glBindTexture(GL_TEXTURE_2D, 0);
         glBindVertexArray(0);

     }
     
    
/*----------------Flying UFO far ----------------------------------------------------------------------*/
   
    //Activate the program for the UFO
    glUseProgram(cubeProgram);
    
    //Set the model matrix
    glm::mat4 ufoModel2= glm::mat4(1.0);
    if(glfwGetTime()<12)
    {
        ufoModel2 = glm::translate(ufoModel2, glm::vec3(10.0f, -1800.0f, -3000.0f));
    }
    else
    {
        ufoModel2 = glm::translate(ufoModel2, glm::vec3(135.0f, 600.0f, -3000.0f));
    }
    ufoModel2 = glm::scale(ufoModel2, glm::vec3(0.5f, 0.5f, 0.5f));
    ufoModel2 = glm::rotate(ufoModel2, (float)(1.5708), glm::vec3(1.0f, 0.0f,  -0.05f));
    
    if(glfwGetTime()<12)
    {
        ufoModel2 = glm::translate(ufoModel2, glm::vec3(10.0f, 40.0f, (float)glfwGetTime() * -400.0f));
    }
    else if(glfwGetTime()>13.7)
    {
        ufoModel2 =  glm::translate(ufoModel, glm::vec3((float)glfwGetTime() * -1.0f, -1000.0f, 1200.0f));
    }
    
    
    glUniformMatrix4fv(modelMatrixPos, 1, GL_FALSE, &ufoModel2[0][0]);
    
    
    //Set the remaining uniforms
    glUniform3fv(lightPositionPos, 1, lightDirection);
    glUniform3f(lightAmbientPos, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
    glUniform3fv(lightDiffusePos, 1, lightDiffuse);
    glUniform3fv(lightSpecularPos, 1, lightSpecular);
    glUniform4fv(materialShininessColorPos, 1, materialShininessColor);
    glUniform1f(materialShininessPos, materialShininess);
    glUniform3fv(cameraPositionPos, 1, cameraPosition);
    
   //Loop through all the meshes loaded from the OBJ-file
    for (int i=0; i<meshes.size(); ++i)
    {
        //Bind the vertex array and texture of the mesh
        glBindVertexArray(meshes[i].arrayNameObject);
        glBindTexture(GL_TEXTURE_2D, meshes[i].textureObject);

        // Draw the vertex array
        glDrawArrays(GL_TRIANGLES, 0, meshes[i].numVerticesObject);

        // Disable vertex array and texture
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }
    
}

void resizeGL(int width, int height)
{
    
    //Prevent division by zero
    if (height == 0)
        height = 1;
    
    glUseProgram(cubeProgram);
    
    //Change the projection matrix
    glm::mat4 proj = glm::perspective(glm::radians(zoom), (float)width/height, 0.1f, 25000.0f);
    
    glUniformMatrix4fv(projectionMatrixPos, 1, GL_FALSE, &proj[0][0]);
    glUseProgram(0);
    
    glUseProgram(reflectCubeProgram);
    glUniformMatrix4fv(reflectProjectionMatrixPos, 1, GL_FALSE, &proj[0][0]);
    glUseProgram(0);
    //Set the  viewport
    glViewport(0, 0, width, height); // 2.0
    
}

//Error callback function for GLFW
static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//Input event callback function for GLFW
static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

//Window size changed callback function for GLFW
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    resizeGL(width, height);
}


int main(void)
{
    //Start the sound engine with default parameters
    ISoundEngine* engine = createIrrKlangDevice();
    
    if (!engine)
    {
        printf("Could not startup engine\n");
        return 0; // error starting up the engine
    }
    
    
    printf("Playing audio track\n");
      printf("Welcome to The Nevada Desert Experience\n");
   
    
    //Plays song track
    engine->play2D("resources/Sound_Track.wav");
    
    //Set error callback
    glfwSetErrorCallback(glfwErrorCallback);
    
    //Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    
    //Specify the OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //Create window
    GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "The Nevada Desert Experience", NULL, NULL);
    if (!window)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    //Set input key event callback
    glfwSetKeyCallback(window, glfwKeyCallback);
    
    //Resize callback
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    
    //Make the context current
    glfwMakeContextCurrent(window);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    
    //Mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    //Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        printf("Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    //Swap buffers
    glfwSwapInterval(0);
    
    //Initialize OpenGL
    if (!initGL())
    {
        printf("Failed to initialize OpenGL\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    //Initialize OpenGL view
    resizeGL(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    //Load the Object
    if (!loadObj("13885_UFO_Triangle_V1_L2.obj")) {
        printf("Failed to load object \n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Runs until the window is closed with ESC
    while (!glfwWindowShouldClose(window))
    {
        //Keyboard input
        processInput(window);
        
        //Mouse input
        glfwSetCursorPosCallback(window, mouse_callback);
        
        // Draw OpenGL screne
        drawGLScene();
        
        // Swap buffers
        glfwSwapBuffers(window);
        
        // Poll fow input events
        glfwPollEvents();
    }
    
    
    //Shutdown GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    //Exit
    exit(EXIT_SUCCESS);
    
}


//Callback function for every keyboard event
void processInput(GLFWwindow *window)
{
    float cameraSpeed = 0.2f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

//Callback function for every mouse event
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.15;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}



//Loading objects
//NB: Do not call this method mutiple times, because it will cause memory leaks on the GPU
int loadObj(const char *objectName) {
    
    //Variables for storing the data in the OBJ-data
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    //Error message from tiny_obj_loader
    std::string errorString;
    
    //Check to se if the object loaded successfully
    if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &errorString, objectName, "."))
        return 0;
    
    //Loop through all the shapes in the object figure
    for(int i=0; i<shapes.size(); ++i) {
        
        //Create a new Mesh
        meshes.push_back(Mesh());
        Mesh *mesh = &meshes[meshes.size()-1];
        
        //Pointer to the mesh of the current shape
        tinyobj::mesh_t *objMesh = &shapes[i].mesh;
        
        //Get the texture of the first face in the mesh.
        std::string texture_objectName = materials[objMesh->material_ids[0]].diffuse_texname;
        
        //Read the texture image
        int width, height, channels;
        GLubyte *imageData = stbi_load(texture_objectName.c_str(), &width, &height, &channels, STBI_default);
        if (!imageData)
            return 0;
        
        //Generate a new mesh and activates it
        glGenTextures(1, &mesh->textureObject);
        glBindTexture(GL_TEXTURE_2D, mesh->textureObject);
        
        //Set the texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        if (channels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        }
        else if (channels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        }
        else
        {
            return 0;
        }
        
        //Generate mip map images
        glGenerateMipmap(GL_TEXTURE_2D);
        
        //Deactivate
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(imageData);
        
        //Store the number of uique vertices and faces in the mesh
        int numVertices = mesh->numVerticesObject = objMesh->indices.size();
        int numFaces = numVertices / 3;
        
        //Create a vector for storing the vector data
        std::vector<GLfloat> vertices;
        
        //Loop through all the faces in the mesh
        for (int f=0; f<numFaces; ++f)
        {
            //Store the indices of the current triangle
            tinyobj::index_t idx1 = objMesh->indices[f * 3];
            tinyobj::index_t idx2 = objMesh->indices[f * 3 + 1];
            tinyobj::index_t idx3 = objMesh->indices[f * 3 + 2];
            
            //Store the first vertex (POSITION NORMAL UV)
            vertices.push_back(attributes.vertices[idx1.vertex_index*3]);
            vertices.push_back(attributes.vertices[idx1.vertex_index*3+1]);
            vertices.push_back(attributes.vertices[idx1.vertex_index*3+2]);
            vertices.push_back(attributes.normals[idx1.normal_index*3]);
            vertices.push_back(attributes.normals[idx1.normal_index*3+1]);
            vertices.push_back(attributes.normals[idx1.normal_index*3+2]);
            vertices.push_back(attributes.texcoords[idx1.texcoord_index*2]);
            vertices.push_back(1.0f - attributes.texcoords[idx1.texcoord_index*2+1]);
            
            //Store the second vertex
            vertices.push_back(attributes.vertices[idx2.vertex_index*3]);
            vertices.push_back(attributes.vertices[idx2.vertex_index*3+1]);
            vertices.push_back(attributes.vertices[idx2.vertex_index*3+2]);
            vertices.push_back(attributes.normals[idx2.normal_index*3]);
            vertices.push_back(attributes.normals[idx2.normal_index*3+1]);
            vertices.push_back(attributes.normals[idx2.normal_index*3+2]);
            vertices.push_back(attributes.texcoords[idx2.texcoord_index*2]);
            vertices.push_back(1.0f - attributes.texcoords[idx2.texcoord_index*2+1]);
            
            //Store the third vertex
            vertices.push_back(attributes.vertices[idx3.vertex_index*3]);
            vertices.push_back(attributes.vertices[idx3.vertex_index*3+1]);
            vertices.push_back(attributes.vertices[idx3.vertex_index*3+2]);
            vertices.push_back(attributes.normals[idx3.normal_index*3]);
            vertices.push_back(attributes.normals[idx3.normal_index*3+1]);
            vertices.push_back(attributes.normals[idx3.normal_index*3+2]);
            vertices.push_back(attributes.texcoords[idx3.texcoord_index*2]);
            vertices.push_back(1.0f - attributes.texcoords[idx3.texcoord_index*2+1]);
            
        }
        
        //Create buffer name for the vertex data
        glGenBuffers(1, &mesh->bufferNameObject); // 2.0
        
        //Allocate storage for the vertex array buffers
        glBindBuffer(GL_ARRAY_BUFFER, mesh->bufferNameObject); // 2.0
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW); // 2.0
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        //Create and initialize a vertex array object
        glGenVertexArrays(1, &mesh->arrayNameObject);
        glBindVertexArray(mesh->arrayNameObject);
        
        //Specify the format of the attributes
        glBindBuffer(GL_ARRAY_BUFFER, mesh->bufferNameObject);
        glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0); // 3.0
        glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(3 * sizeof(GL_FLOAT))); // 3.0
        glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void *)(6 * sizeof(GL_FLOAT))); // 3.0
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        //Enable the attributes for the object
        glEnableVertexAttribArray(POSITION);
        glEnableVertexAttribArray(NORMAL);
        glEnableVertexAttribArray(UV);
        
        glBindVertexArray(0);
        
    }
    
    return 1;
}


