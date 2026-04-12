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
    
    // Calculate distance to nearest edge
    float minDist = min(vBarycentric.x, min(vBarycentric.y, vBarycentric.z));
    
    // Draw wireframe only on the actual edges
    // Use a small threshold (0.01-0.05) and make sure it's exactly on edges
    if (minDist < wireframeWidth) {
        FragColor = vec4(wireframeColor, 1.0);
    } else {
        FragColor = solidColor;
    }
}
