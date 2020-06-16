#version 330 core
layout (location = 0) in vec3 position;

out vec3 textCoords;

uniform mat4 landscapeView;
uniform mat4 landscapeProj;


void main()
{
    textCoords = position;
    vec4 pos = landscapeProj * landscapeView * vec4(position, 1.0);
    gl_Position = pos.xyww;
   
    
}
