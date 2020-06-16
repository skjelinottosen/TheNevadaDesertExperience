 
#version 330 core

// Incoming vertex position, uv and normal
layout (location = 0) in vec3 position;
layout (location = 1 ) in vec2 uv;
layout (location = 2) in vec3 normal;


// Projection, view and model matrices.
uniform mat4 proj;
uniform mat4 view;
uniform mat4 reflectModel;

// Output variables
out vec2 UV;
out vec3 N;
out vec3 vecPosInWorld;

// Outgoing color.
void main()
{
    // Normally gl_Position is in Clip Space and we calculate it by multiplying together all the matrices
    gl_Position = proj * view * reflectModel * vec4(position, 1);
    // Set the world vertex for calculating the light direction in the fragment shader
    vecPosInWorld = vec3(reflectModel * vec4(position, 1));
    // Set the transformed normal
    N = mat3(transpose(inverse(reflectModel))) * normal;
    
    UV = uv;
}



