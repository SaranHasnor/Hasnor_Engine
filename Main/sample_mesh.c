#include <engine.h>
#include <utils_matrix.h>
#include <utils_math.h>

mesh_t *_texturedMesh;
mesh_t *_animatedMesh;
mesh_t *_geometryShaderMesh;
mesh_t *_sky;

const char *_geomShaderTest =
	"#version 330\n	layout(triangles) in;			\
	layout(triangle_strip, max_vertices=6) out;			\
	void main(void)									\
	{												\
		gl_Position = gl_in[0].gl_Position;			\
		EmitVertex();								\
		gl_Position = gl_in[1].gl_Position;			\
		EmitVertex();								\
		gl_Position = gl_in[2].gl_Position;			\
		EmitVertex();								\
		EndPrimitive();								\
		gl_Position = gl_in[0].gl_Position;			\
		EmitVertex();								\
		gl_Position = gl_in[1].gl_Position;			\
		EmitVertex();								\
		gl_Position = gl_in[2].gl_Position - vec4(0.0, 5.0, 0.0, 0.0);			\
		EmitVertex();								\
		EndPrimitive();								\
	}";

#define Geom Engine.Render.Geometry
#define Prog Engine.Render.Program
void initSampleMesh(void)
{
	texture_t *texture = Engine.Render.Texture.fromPath("textures/Lotus.jpg");

	_texturedMesh = Geom.newMesh();
	Geom.addFace()->texture = texture;
	Geom.addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	Geom.addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Geom.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	Geom.addFace()->texture = texture;
	Geom.addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	Geom.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Geom.addVertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	Geom.updateMeshGeometry(_texturedMesh);

	Vector.set(_texturedMesh->origin, -2.0f, -1.5f, 0.0f);
	Matrix.rotation(_texturedMesh->rotation, -90.0f, 45.0f, 0.0f, true);

	_animatedMesh = Geom.newMesh();
	Geom.addFace();
	Geom.addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	Geom.addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Geom.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	Geom.addFace();
	Geom.addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	Geom.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Geom.addVertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	Geom.updateMeshGeometry(_animatedMesh);

	_sky = NULL; // TODO

	_geometryShaderMesh = Geom.newMesh();
	Geom.addFace()->program = Prog.withShaders(Prog.getDefault(false)->vertexShader,
		Prog.getDefault(false)->fragmentShader,
		Engine.Render.Shader.fromContent(SHADER_GEOMETRY, _geomShaderTest));

	Geom.addVertex(-1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Geom.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Geom.addVertex(0.0f, 0.0f, 1.0f, 0.5f, 1.0f);
	Geom.updateMeshGeometry(_geometryShaderMesh);
	_geometryShaderMesh->origin[2] = 2.0f;
}


void updateSampleMesh(timeStruct_t time, inputStruct_t input)
{
	uint i;
	_animatedMesh->origin[0] = Math.pi * Math.cos(((time.currentTime / 20) & 255) * Math.pi * 2.0f / 255.0f);
	Matrix.rotation(_animatedMesh->rotation, _animatedMesh->origin[0], 0.0f, 0.0f, false);
	for (i = 0; i < _animatedMesh->nbFaces; i++)
	{
		if (_animatedMesh->origin[0] > 0)
		{
			_animatedMesh->faces[i]->color[0] = 1.0f;
			_animatedMesh->faces[i]->color[1] = 1.0f - _animatedMesh->origin[0] / Math.pi;
			_animatedMesh->faces[i]->color[2] = 1.0f - _animatedMesh->origin[0] / Math.pi;
		}
		else
		{
			_animatedMesh->faces[i]->color[0] = 1.0f;
			_animatedMesh->faces[i]->color[1] = 1.0f;
			_animatedMesh->faces[i]->color[2] = 1.0f + _animatedMesh->origin[0] / Math.pi;
		}
	}
}

void drawSampleMesh(float viewMatrix[16])
{
	//Geom.renderMesh(_sky, viewMatrix);
	Geom.renderMesh(_texturedMesh, viewMatrix);
	Geom.renderMesh(_animatedMesh, viewMatrix);
	Geom.renderMesh(_geometryShaderMesh, viewMatrix);
}
