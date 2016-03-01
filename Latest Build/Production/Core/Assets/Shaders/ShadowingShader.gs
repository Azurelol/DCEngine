#version 330 core

layout (lines) in; // 2 vertices in
layout (triangle_strip, max_vertices = 4) out;

in vec3 WorldPos[]; // an array of 3 vertices (triangle with adjacency)

struct Light
{
	bool Visible;
	bool VisibilityCulling;
	bool VisibilityEvents;
	bool CastShadows;
	int LightType;
	vec4 Color;
	float Intensity;
	float Range;
	float Falloff;
	vec3 Direction;
	float InnerAngle;
	float OuterAngle;
	vec3 Position;
	mat4 Model;
};
uniform bool useLight;
uniform Light gLight;
uniform mat4 view;
uniform mat4 projection;

float EPSILON = 0.0001;

// Emit a quad using a triangle strip
void EmitQuad(vec3 StartVertex, vec3 EndVertex, mat4 projview)
{
    // Vertex #1: the starting vertex (just a tiny bit below the original edge)
    vec3 LightDir = normalize(StartVertex - gLight.Position); 
    gl_Position = projview * vec4(StartVertex, 1.0);
    EmitVertex();

    // Vertex #2: the starting vertex projected to infinity
    gl_Position = projview vec4(LightDir, 0.0);
    EmitVertex();

    // Vertex #3: the ending vertex (just a tiny bit below the original edge)
    LightDir = normalize(EndVertex - gLight.Position);
    gl_Position = projview * vec4(EndVertex, 1.0);
    EmitVertex();

    // Vertex #4: the ending vertex projected to infinity
    gl_Position = projview vec4(LightDir, 0.0);
    EmitVertex();

    EndPrimitive(); 
}


void main()
{
	mat4 projview = projection * view;
  //float len1 = length(WorldPos[0] - gLight.Position);
  //float len2 = length(WorldPos[1] - gLight.Position);
  //if(gLight.Range > len1 || gLight.Range > len2)
	  EmitQuad(WorldPos[0], WorldPos[1], projview);
	//EmitQuad(WorldPos[1], WorldPos[2]);
	//EmitQuad(WorldPos[2], WorldPos[0]);
}