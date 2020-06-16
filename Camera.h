#ifndef Camera_h
#define Camera_h

//Camera position
glm::vec3 cameraPos   = glm::vec3(-12.0f, 6.0f, 40.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//Mouse
bool firstMouse = true;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float zoom   =  45.0f;

#endif /* Camera_h */
