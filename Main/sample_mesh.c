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

void initSampleMesh(void)
{
	texture_t *texture = GLTexture.fromPath("textures/Lotus.jpg");

	_texturedMesh = Mesh.newMesh();
	Mesh.addFace()->texture = texture;
	Mesh.addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	Mesh.addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Mesh.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	Mesh.addFace()->texture = texture;
	Mesh.addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	Mesh.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Mesh.addVertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	Mesh.updateGeometry(_texturedMesh);

	Vector.set(_texturedMesh->origin, -2.0f, -1.5f, 0.0f);
	Matrix.rotation(_texturedMesh->rotation, -90.0f, 45.0f, 0.0f, true);

	_animatedMesh = Mesh.newMesh();
	Mesh.addFace();
	Mesh.addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	Mesh.addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Mesh.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	Mesh.addFace();
	Mesh.addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	Mesh.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Mesh.addVertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	Mesh.updateGeometry(_animatedMesh);

	_sky = NULL; // TODO

	_geometryShaderMesh = Mesh.newMesh();
	Mesh.addFace()->program = GLProgram.withShaders(GLProgram.getDefault(false)->vertexShader,
		GLProgram.getDefault(false)->fragmentShader,
		GLShader.fromContent(SHADER_GEOMETRY, _geomShaderTest));

	Mesh.addVertex(-1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Mesh.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Mesh.addVertex(0.0f, 0.0f, 1.0f, 0.5f, 1.0f);
	Mesh.updateGeometry(_geometryShaderMesh);
	_geometryShaderMesh->origin[2] = 2.0f;
}


void updateSampleMesh(const timeStruct_t time, const inputStruct_t input)
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

void drawSampleMesh(const float viewMatrix[16])
{
	//Mesh.render(_sky, viewMatrix);
	Mesh.render(_texturedMesh, viewMatrix);
	Mesh.render(_animatedMesh, viewMatrix);
	Mesh.render(_geometryShaderMesh, viewMatrix);
}
