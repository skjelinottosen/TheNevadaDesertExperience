#version 330 core

in vec3 textCoords;
out vec4 outColor;

uniform samplerCube landscape;

void main()
{
    outColor = texture(landscape, textCoords);
}
