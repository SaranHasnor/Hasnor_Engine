

#include <engine.h>
#include <utils_math.h>

mesh_t *_texturedMesh;
mesh_t *_animatedMesh;
mesh_t *_sky;

void initSampleMesh(void)
{
	texture_t *texture = Engine.Render.Texture.fromPath("textures/Lotus.jpg");

	_texturedMesh = Engine.Render.Geometry.newMesh();
	Engine.Render.Geometry.addFace()->texture = texture;
	Engine.Render.Geometry.addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	Engine.Render.Geometry.addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Engine.Render.Geometry.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	Engine.Render.Geometry.addFace()->texture = texture;
	Engine.Render.Geometry.addVertex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	Engine.Render.Geometry.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Engine.Render.Geometry.addVertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	Engine.Render.Geometry.updateMeshGeometry(_texturedMesh);

	Vector.set(_texturedMesh->origin, -2.0f, -1.5f, 0.0f);
	Matrix.rotation(_texturedMesh->rotation, -90.0f, 45.0f, 0.0f, true);

	_animatedMesh = Engine.Render.Geometry.newMesh();
	Engine.Render.Geometry.addFace();
	Engine.Render.Geometry.addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	Engine.Render.Geometry.addVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Engine.Render.Geometry.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		
	Engine.Render.Geometry.addFace();
	Engine.Render.Geometry.addVertex(0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	Engine.Render.Geometry.addVertex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	Engine.Render.Geometry.addVertex(1.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	Engine.Render.Geometry.updateMeshGeometry(_animatedMesh);

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
	//Engine.Render.Geometry.renderMesh(_sky, viewMatrix);
	Engine.Render.Geometry.renderMesh(_texturedMesh, viewMatrix);
	Engine.Render.Geometry.renderMesh(_animatedMesh, viewMatrix);
}