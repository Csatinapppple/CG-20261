#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 TexCoords[];
out vec2 vTexCoords;
out vec3 vBarycentric;

void main() {
    for(int i = 0; i < 3; i++) {
        vTexCoords = TexCoords[i];
        
        vec3 barycentric = vec3(0.0);
        barycentric[i] = 1.0;
        vBarycentric = barycentric;
        
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
