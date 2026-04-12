#version 330 core
out vec4 FragColor;

in vec2 vTexCoords;
in vec3 vBarycentric;

uniform sampler2D texture_diffuse1;
uniform bool wireframe;
uniform vec3 wireframeColor;
uniform float wireframeWidth;

void main()
{
    vec4 solidColor = texture(texture_diffuse1, vTexCoords);
    
    if (!wireframe) {
        FragColor = solidColor;
        return;
    }
    
    float minDist = min(vBarycentric.x, min(vBarycentric.y, vBarycentric.z));
    
    if (minDist < wireframeWidth) {
        FragColor = vec4(wireframeColor, 1.0);
    } else {
        FragColor = solidColor;
    }
}
