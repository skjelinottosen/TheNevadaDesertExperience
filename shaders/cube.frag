#version 330 core


//Incoming variables.
in vec2 UV;
in vec3 N;
in vec3 vecPosInWorld;

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec4 shininessColor;
uniform float shininess;
uniform vec3 cameraPosition;

//Outgoing final color.
out vec4 outputColor;


//Vectors
vec3 L;
vec3 NN;
vec3 V;
vec3 R;
vec3 normal;

//Light
vec4 color;
vec4 ambient;
vec4 diffuse;
vec4 specular;

//Texture sampler
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;



void main()
{
    normal = texture(normalMap, UV).rgb;
          // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

        
    color = texture(diffuseMap, UV).rgba;
    
   
    // Normalize the interpolated normal to ensure unit length
    NN = normalize(normal);
    
    // Find the unit length normal giving the direction from the vertex to the light
    L = normalize(lightDirection - vecPosInWorld);

    // Find the unit length normal giving the direction from the vertex to the camera
    V = normalize(cameraPosition - vecPosInWorld);

    // Find the unit length reflection normal
    R = normalize(reflect(-L, NN));
    
    // Calculate the ambient component
    ambient = vec4(lightAmbient, 1) * color;

    // Calculate the diffuse component
    diffuse = vec4(max(dot(L, NN), 0.0) * lightDiffuse, 1) * color;

    // Calculate the specular component
    specular = vec4(pow(max(dot(R, V), 0.0), shininess) * lightSpecular, 1) * shininessColor;

    // Put it all together
    outputColor = ambient + diffuse + specular;
   
   
}

