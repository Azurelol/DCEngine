#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in vec3 WorldCoords[];

const int MaxNumLights = 20;

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

uniform Light Lights[MaxNumLights];
uniform int numLights;
uniform mat4 view;
uniform mat4 projection;

void EmitQuad(vec3 StartVertex, vec3 EndVertex, vec3 LightPos, float LightRange, mat4 VP)
{
  // Vertex #1: the starting vertex (just a tiny bit below the original edge)
  vec3 LightDir = normalize(StartVertex - LightPos); 
  gl_Position = VP * vec4(StartVertex, 1);
  EmitVertex();

  // Vertex #2: the starting vertex projected to infinity
  gl_Position = VP * vec4(LightDir * LightRange, 1) + vec4(StartVertex, 1);
  EmitVertex();

  // Vertex #3: the ending vertex (just a tiny bit below the original edge)
  LightDir = normalize(EndVertex - LightPos);
  gl_Position = VP * vec4(EndVertex, 1);
  EmitVertex();

  // Vertex #4: the ending vertex projected to infinity
  gl_Position = VP * vec4(LightDir * LightRange, 1) + vec4(EndVertex, 1);
  EmitVertex();

  EndPrimitive(); 
}

void main()
{
	vec3 e1 = WorldCoords[1] - WorldCoords[0];
	vec3 e2 = WorldCoords[2] - WorldCoords[0];
	vec3 normal = cross(e1, e2);
	mat4 VP = projection * view;
	for(int i = 0; i < numLights; ++i)
	{
		Light light = Lights[i];
		if(dot(normal, WorldCoords[0] - light.Position) >= 0)
		{
			EmitQuad(WorldCoords[0], WorldCoords[1], light.Position, light.Range, VP);
			EmitQuad(WorldCoords[1], WorldCoords[2], light.Position, light.Range, VP);
			EmitQuad(WorldCoords[2], WorldCoords[0], light.Position, light.Range, VP);
		}

	}
	//gl_Position = vec4(WorldCoords[0], WorldCoords[1]);
	//EmitVertex();
	//gl_Position = vec4(WorldCoords[1], WorldCoords[2]);
	//EmitVertex();
	//gl_Position = vec4(WorldCoords[2], WorldCoords[0]);
	//EmitVertex();
}