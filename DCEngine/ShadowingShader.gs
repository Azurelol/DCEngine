#version 330

layout (triangles) in; // 3 vertices in
layout (triangle_strip, max_vertices = 12) out;

in vec3 WorldPos[]; // an array of 6 vertices (triangle with adjacency)

uniform bool useLight;
uniform vec3 gLightPos;
uniform mat4 gVP;

float EPSILON = 0.0001;

// Emit a quad using a triangle strip
void EmitQuad(vec3 StartVertex, vec3 EndVertex)
{
    // Vertex #1: the starting vertex (just a tiny bit below the original edge)
    vec3 LightDir = normalize(StartVertex - gLightPos); 
    gl_Position = vec4(StartVertex + LightDir * EPSILON, 1.0);
    EmitVertex();

    // Vertex #2: the starting vertex projected to infinity
    gl_Position = vec4(LightDir, 0.0);
    EmitVertex();

    // Vertex #3: the ending vertex (just a tiny bit below the original edge)
    LightDir = normalize(EndVertex - gLightPos);
    gl_Position = vec4((EndVertex + LightDir * EPSILON), 1.0);
    EmitVertex();

    // Vertex #4: the ending vertex projected to infinity
    gl_Position = vec4(LightDir , 0.0);
    EmitVertex();

    EndPrimitive(); 
}


void main()
{
	EmitQuad(WorldPos[0], WorldPos[1]);
	EmitQuad(WorldPos[1], WorldPos[2]);
	EmitQuad(WorldPos[2], WorldPos[0]);
}