#version 330 core


//Incoming variables.
in vec2 UV;
in vec3 N;
in vec3 vecPosInWorld;


//Light
vec4 color;
vec4 ambient;

//Outgoing final color.
out vec4 outputColor;

uniform vec3 lightAmbient;

//Texture sampler
uniform vec3 cameraPosition;

uniform samplerCube landscape;


void main()
{
     //color = texture(diffuseMap, UV).rgba;
    // Calculate the ambient component
    ambient = vec4(lightAmbient, 1) * color;
    
    vec3 I = normalize(vecPosInWorld - cameraPosition);
    vec3 R = reflect(I, normalize(N));
    outputColor = vec4(texture(landscape, I).rgb, 1.0);
   
   
}

