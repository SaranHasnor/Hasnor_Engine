

#include <engine_callbacks.h>
#include <engine_render.h>
#include <utils_math.h>

mesh_t *_texturedMesh;
mesh_t *_animatedMesh;
mesh_t *_sky;

void initSampleMesh()
{
	texture_t *texture = textureFromPath("textures/Lotus.jpg");

	_texturedMesh = newMesh();
	addFace()->texture = texture;
	addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	addFace()->texture = texture;
	addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	addVertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	updateMeshGeometry(_texturedMesh);

	Vector.set(_texturedMesh->origin, -2.0f, -1.5f, 0.0f);
	Matrix.rotation(_texturedMesh->rotation, -90.0f, 45.0f, 0.0f, true);

	_animatedMesh = newMesh();
	addFace();
	addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	addFace();
	addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	addVertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	updateMeshGeometry(_animatedMesh);

	_sky = NULL; // TODO
}


void updateSampleMesh(timeStruct_t time, inputStruct_t input)
{
	uint i;
	_animatedMesh->origin[0] = (float)M_PI * cosf(((time.currentTime / 20) & 255) * M_PI * 2.0f / 255.0f);
	Matrix.rotation(_animatedMesh->rotation, _animatedMesh->origin[0], 0.0f, 0.0f, false);
	for (i = 0; i < _animatedMesh->nbFaces; i++)
	{
		if (_animatedMesh->origin[0] > 0)
		{
			_animatedMesh->faces[i]->color[0] = 1.0f;
			_animatedMesh->faces[i]->color[1] = 1.0f - _animatedMesh->origin[0] / (float)M_PI;
			_animatedMesh->faces[i]->color[2] = 1.0f - _animatedMesh->origin[0] / (float)M_PI;
		}
		else
		{
			_animatedMesh->faces[i]->color[0] = 1.0f;
			_animatedMesh->faces[i]->color[1] = 1.0f;
			_animatedMesh->faces[i]->color[2] = 1.0f + _animatedMesh->origin[0] / (float)M_PI;
		}
	}
}

void drawSampleMesh(float viewMatrix[16])
{
	//renderMesh(_sky, viewMatrix);
	renderMesh(_texturedMesh, viewMatrix);
	renderMesh(_animatedMesh, viewMatrix);
}