#define HASNOR_ENGINE_INTERNAL

#include "engine_geometry.h"
#include <utils_math.h>

// engine_mesh.c global variables, we're basically an extension so let's say it's fine to see them
extern mesh_t *_selectedMesh;
extern face_t *_selectedFace;
extern float _cursorPos[3];
extern float _localAxis[3][3];
extern bool _autoSelect;

mesh_t *createSphere(float radius, int rings, int sectors, texture_t *texture, program_t *program)
{ // Inspired by http://stackoverflow.com/questions/7946770/calculating-a-sphere-in-opengl
	const float R = 1.0f / (rings-1);
	const float S = 1.0f / (sectors-1);
	int r, s;
	vertex_t *vertices = allocArray(vertex_t, rings * sectors);
	vertex_t *vertexIterator = vertices;
	mesh_t *sphere;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			const float x = Math.cos(2.0f * Math.pi * s * S) * Math.sin(Math.pi * r * R);
			const float y = Math.sin(2.0f * Math.pi * s * S) * Math.sin(Math.pi * r * R);
			const float z = Math.sin(-0.5f * Math.pi + Math.pi * r * R);

			vertexIterator->coords.x = _cursorPos[0] + x * radius;
			vertexIterator->coords.y = _cursorPos[1] + y * radius;
			vertexIterator->coords.z = _cursorPos[2] + z * radius;

			Vector.rotate((float*)&vertexIterator->coords, _localAxis);

			vertexIterator->texCoords.u = s * S;
			vertexIterator->texCoords.v = 1.0f - r * R;

			vertexIterator->normal.x = x;
			vertexIterator->normal.y = y;
			vertexIterator->normal.z = z;

			vertexIterator++;
		}
	}

	sphere = Mesh.newMesh();

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			face_t *currentFace;
			vertex_t **currentVertices;
			vertex_t *v1 = &vertices[((r+0)%rings) * sectors + ((s+0)%sectors)];
			vertex_t *v2 = &vertices[((r+0)%rings) * sectors + ((s+1)%sectors)];
			vertex_t *v3 = &vertices[((r+1)%rings) * sectors + ((s+1)%sectors)];
			vertex_t *v4 = &vertices[((r+1)%rings) * sectors + ((s+0)%sectors)];

			currentFace = Mesh.addFaceToMesh(sphere);
			currentFace->texture = texture;
			currentFace->program = program;
			currentVertices = allocArray(vertex_t*, 3);
			currentVertices[0] = v1;
			currentVertices[1] = v2;
			currentVertices[2] = v3;
			currentFace->vertices = currentVertices;
			currentFace->nbVertices = 3;

			currentFace = Mesh.addFaceToMesh(sphere);
			currentFace->texture = texture;
			currentFace->program = program;
			currentVertices = allocArray(vertex_t*, 3);
			currentVertices[0] = v3;
			currentVertices[1] = v4;
			currentVertices[2] = v1;
			currentFace->vertices = currentVertices;
			currentFace->nbVertices = 3;
		}
	}

	Mesh.updateGeometry(sphere);

	return sphere;
}

mesh_t *createRing(float innerRadius, float outerRadius, int sectors, texture_t *texture, program_t *program)
{
	const float S = 1.0f / (sectors-1);
	const int vertexCount = 2 * sectors;
	int s;
	vertex_t *vertices = allocArray(vertex_t, vertexCount);
	vertex_t *vertexIterator = vertices;
	mesh_t *ring;

	for (s = 0; s < sectors; s++)
	{
		const float x = Math.cos(2.0f * Math.pi * s * S);
		const float y = Math.sin(2.0f * Math.pi * s * S);
		const float z = 0.0f;

		vertexIterator->coords.x = _cursorPos[0] + x * innerRadius;
		vertexIterator->coords.y = _cursorPos[1] + y * innerRadius;
		vertexIterator->coords.z = _cursorPos[2] + z * innerRadius;

		Vector.rotate((float*)&vertexIterator->coords, _localAxis);

		vertexIterator->texCoords.u = 1.0f;
		vertexIterator->texCoords.v = 0.0f;

		vertexIterator->normal.x = x;
		vertexIterator->normal.y = y;
		vertexIterator->normal.z = z;

		vertexIterator++;

		vertexIterator->coords.x = _cursorPos[0] + x * outerRadius;
		vertexIterator->coords.y = _cursorPos[1] + y * outerRadius;
		vertexIterator->coords.z = _cursorPos[2] + z * outerRadius;

		Vector.rotate((float*)&vertexIterator->coords, _localAxis);

		vertexIterator->texCoords.u = 0.0f;
		vertexIterator->texCoords.v = 0.0f;

		vertexIterator->normal.x = x;
		vertexIterator->normal.y = y;
		vertexIterator->normal.z = z;

		vertexIterator++;
	}

	ring = Mesh.newMesh();

	for (s = 0; s < sectors; s++)
	{
		face_t *currentFace;
		vertex_t **currentVertices;
		vertex_t *v1 = &vertices[(2 * s + 0) % (vertexCount)];
		vertex_t *v2 = &vertices[(2 * s + 1) % (vertexCount)];
		vertex_t *v3 = &vertices[(2 * s + 3) % (vertexCount)];
		vertex_t *v4 = &vertices[(2 * s + 2) % (vertexCount)];

		currentFace = Mesh.addFaceToMesh(ring);
		currentFace->texture = texture;
		currentFace->program = program;
		currentVertices = allocArray(vertex_t*, 3);
		currentVertices[0] = v1;
		currentVertices[1] = v2;
		currentVertices[2] = v3;
		currentFace->vertices = currentVertices;
		currentFace->nbVertices = 3;

		currentFace = Mesh.addFaceToMesh(ring);
		currentFace->texture = texture;
		currentFace->program = program;
		currentVertices = allocArray(vertex_t*, 3);
		currentVertices[0] = v3;
		currentVertices[1] = v4;
		currentVertices[2] = v1;
		currentFace->vertices = currentVertices;
		currentFace->nbVertices = 3;
	}

	Mesh.updateGeometry(ring);

	return ring;
}

void initGeometryFunctions(void)
{
	Primitives.sphere = createSphere;
	Primitives.ring = createRing;
}

#undef HASNOR_ENGINE_INTERNAL
