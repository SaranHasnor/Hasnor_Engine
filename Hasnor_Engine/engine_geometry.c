#define HASNOR_ENGINE_INTERNAL

#include "engine_geometry.h"
#include <utils_matrix.h>
#include <utils_quaternion.h>
#include <utils_math.h>

#include <GL/glew.h>

/*
engine_geometry

- 3D mesh generation and manipulation functions
- TODO: Primitives, index saving when rendering
*/

mesh_t *_selectedMesh = NULL;
face_t *_selectedFace = NULL;

float _cursorPos[3] = {0.0f, 0.0f, 0.0f};
float _localAxis[3][3] = {{1.0f, 0.0f, 0.0f},
						  {0.0f, 1.0f, 0.0f},
						  {0.0f, 0.0f, 1.0f}};

bool _autoSelect = true;

void linkVertexToFace(face_t *face, vertex_t *vertex)
{
#ifdef _DEBUG
	uint i=0;
	while (i<face->nbVertices)
	{
		if (face->vertices[i] == vertex)
		{
			printf("ERROR: Vertex is already in face!\n");
			return;
		}
		i++;
	}
#endif

	face->vertices = (vertex_t**)Memory.realloc(face->vertices, sizeof(vertex_t*) * (face->nbVertices+1));

	face->vertices[face->nbVertices++] = vertex;
}

vertex_t *_addVertexToFace(face_t *face, float pos[3], float u, float v)
{
	vertex_t *vertex = newObject(vertex_t);
	Vector.rotate(pos, _localAxis);
	vertex->coords.x = pos[0] + _cursorPos[0];
	vertex->coords.y = pos[1] + _cursorPos[1];
	vertex->coords.z = pos[2] + _cursorPos[2];
	vertex->texCoords.u = u;
	vertex->texCoords.v = v;
	vertex->normal.x = 0.0f;
	vertex->normal.y = 0.0f;
	vertex->normal.z = 0.0f;
	linkVertexToFace(face, vertex);
	return vertex;
}

vertex_t *addVertexToFace(face_t *face, float x, float y, float z, float u, float v)
{
	float pos[3];
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	return _addVertexToFace(face, pos, u, v);
}

vertex_t *addVertex(float x, float y, float z, float u, float v)
{ // Adds a vertex to the selected face
	if (_selectedFace)
	{
		return addVertexToFace(_selectedFace, x, y, z, u, v);
	}
	return NULL;
}

void linkFaceToMesh(mesh_t *mesh, face_t *face)
{
#ifdef _DEBUG
	uint i=0;
	while (i<mesh->nbFaces)
	{
		if (mesh->faces[i] == face)
		{
			printf("ERROR: Face is already in mesh!\n");
			return;
		}
		i++;
	}
#endif

	mesh->faces = (face_t**)Memory.realloc(mesh->faces, sizeof(face_t*) * (mesh->nbFaces+1));

	mesh->faces[mesh->nbFaces++] = face;
}

face_t *addFaceToMesh(mesh_t *mesh)
{
	face_t *face = newObject(face_t);
	Memory.set(face, 0, sizeof(face_t));

	glGenBuffers(1, &face->vboIndex);
	glGenBuffers(1, &face->eboIndex);

	Vector.set(face->color, 1.0f, 1.0f, 1.0f);
	face->color[3] = 1.0f;

	linkFaceToMesh(mesh, face);

	if (_autoSelect) _selectedFace = face;

	return face;
}

face_t *addFace(void)
{ // Adds a face to the currently selected mesh
	if (_selectedMesh)
	{
		return addFaceToMesh(_selectedMesh);
	}
	return NULL;
}

mesh_t *newMesh(void)
{
	mesh_t *mesh = newObject(mesh_t);
	Memory.set(mesh, 0, sizeof(mesh_t));

	//glGenBuffers(1, &mesh->vboIndex);
	//glGenBuffers(1, &mesh->eboIndex);

	Matrix.identity(mesh->rotation);

	if (_autoSelect) _selectedMesh = mesh;

	return mesh;
}

vertex_t *duplicateVertex(vertex_t *vertex)
{
	vertex_t *newVertex = (vertex_t*)Memory.duplicate(vertex, sizeof(vertex_t));
	return newVertex;
}

face_t *duplicateFace(face_t *face)
{
	face_t *newFace = newObject(face_t);
	uint i;

	newFace->nbVertices = face->nbVertices;
	newFace->vertices = newArray(vertex_t*, newFace->nbVertices);

	for (i=0; i<newFace->nbVertices; i++)
	{
		newFace->vertices[i] = duplicateVertex(face->vertices[i]);
	}

	return newFace;
}

mesh_t *duplicateMesh(mesh_t *mesh)
{
	mesh_t *newMesh = newObject(mesh_t);
	uint i;

	newMesh->nbFaces = mesh->nbFaces;
	newMesh->faces = newArray(face_t*, newMesh->nbFaces);

	for (i=0; i<newMesh->nbFaces; i++)
	{
		newMesh->faces[i] = duplicateFace(mesh->faces[i]);
	}

	return newMesh;
}

void updateMeshGeometry(mesh_t *mesh);
mesh_t *createSphere(float center[3], float radius, int rings, int sectors, texture_t *texture, program_t *program)
{ // Inspired by http://stackoverflow.com/questions/7946770/calculating-a-sphere-in-opengl
	const float R = 1.0f / (rings-1);
	const float S = 1.0f / (sectors-1);
	int r, s;
	vertex_t *vertices = newArray(vertex_t, rings * sectors);
	vertex_t *vertexIterator = vertices;
	mesh_t *sphere;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			const float x = Math.cos(2.0f * Math.pi * s * S) * Math.sin(Math.pi * r * R);
			const float y = Math.sin(2.0f * Math.pi * s * S) * Math.sin(Math.pi * r * R);
			const float z = Math.sin(-0.5f * Math.pi + Math.pi * r * R);

			vertexIterator->coords.x = center[0] + x * radius;
			vertexIterator->coords.y = center[1] + y * radius;
			vertexIterator->coords.z = center[2] + z * radius;

			Vector.rotate((float*)&vertexIterator->coords, _localAxis);

			vertexIterator->texCoords.u = s * S;
			vertexIterator->texCoords.v = 1.0f - r * R;

			vertexIterator->normal.x = x;
			vertexIterator->normal.y = y;
			vertexIterator->normal.z = z;

			vertexIterator++;
		}
	}

	sphere = newMesh();

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

			currentFace = addFaceToMesh(sphere);
			currentFace->texture = texture;
			currentFace->program = program;
			currentVertices = newArray(vertex_t*, 3);
			currentVertices[0] = v1;
			currentVertices[1] = v2;
			currentVertices[2] = v3;
			currentFace->vertices = currentVertices;
			currentFace->nbVertices = 3;

			currentFace = addFaceToMesh(sphere);
			currentFace->texture = texture;
			currentFace->program = program;
			currentVertices = newArray(vertex_t*, 3);
			currentVertices[0] = v3;
			currentVertices[1] = v4;
			currentVertices[2] = v1;
			currentFace->vertices = currentVertices;
			currentFace->nbVertices = 3;
		}
	}

	updateMeshGeometry(sphere);

	return sphere;
}

mesh_t *createRing(float center[3], float innerRadius, float outerRadius, int sectors, texture_t *texture, program_t *program)
{
	const float S = 1.0f / (sectors-1);
	const int vertexCount = 2 * sectors;
	int s;
	vertex_t *vertices = newArray(vertex_t, vertexCount);
	vertex_t *vertexIterator = vertices;
	mesh_t *ring;

	for (s = 0; s < sectors; s++)
	{
		const float x = Math.cos(2.0f * Math.pi * s * S);
		const float y = Math.sin(2.0f * Math.pi * s * S);
		const float z = 0.0f;

		vertexIterator->coords.x = center[0] + x * innerRadius;
		vertexIterator->coords.y = center[1] + y * innerRadius;
		vertexIterator->coords.z = center[2] + z * innerRadius;

		Vector.rotate((float*)&vertexIterator->coords, _localAxis);

		vertexIterator->texCoords.u = 1.0f;
		vertexIterator->texCoords.v = 0.0f;

		vertexIterator->normal.x = x;
		vertexIterator->normal.y = y;
		vertexIterator->normal.z = z;

		vertexIterator++;

		vertexIterator->coords.x = center[0] + x * outerRadius;
		vertexIterator->coords.y = center[1] + y * outerRadius;
		vertexIterator->coords.z = center[2] + z * outerRadius;

		Vector.rotate((float*)&vertexIterator->coords, _localAxis);

		vertexIterator->texCoords.u = 0.0f;
		vertexIterator->texCoords.v = 0.0f;

		vertexIterator->normal.x = x;
		vertexIterator->normal.y = y;
		vertexIterator->normal.z = z;

		vertexIterator++;
	}

	ring = newMesh();

	for (s = 0; s < sectors; s++)
	{
		face_t *currentFace;
		vertex_t **currentVertices;
		vertex_t *v1 = &vertices[(2 * s + 0) % (vertexCount)];
		vertex_t *v2 = &vertices[(2 * s + 1) % (vertexCount)];
		vertex_t *v3 = &vertices[(2 * s + 3) % (vertexCount)];
		vertex_t *v4 = &vertices[(2 * s + 2) % (vertexCount)];

		currentFace = addFaceToMesh(ring);
		currentFace->texture = texture;
		currentFace->program = program;
		currentVertices = newArray(vertex_t*, 3);
		currentVertices[0] = v1;
		currentVertices[1] = v2;
		currentVertices[2] = v3;
		currentFace->vertices = currentVertices;
		currentFace->nbVertices = 3;

		currentFace = addFaceToMesh(ring);
		currentFace->texture = texture;
		currentFace->program = program;
		currentVertices = newArray(vertex_t*, 3);
		currentVertices[0] = v3;
		currentVertices[1] = v4;
		currentVertices[2] = v1;
		currentFace->vertices = currentVertices;
		currentFace->nbVertices = 3;
	}

	updateMeshGeometry(ring);

	return ring;
}

void destroyVertex(vertex_t *vertex)
{
	Memory.freeSafe(vertex);
}

void destroyFace(face_t *face)
{
	uint i;
	for (i=0; i<face->nbVertices; i++)
	{
		destroyVertex(face->vertices[i]);
	}

	glDeleteBuffers(1, &face->vboIndex);
	glDeleteBuffers(1, &face->eboIndex);

	if (face->vbo)
	{
		destroy(face->vbo);
	}
	if (face->ebo)
	{
		destroy(face->ebo);
	}

	if (face->vertices)
	{
		destroy(face->vertices);
	}
	destroy(face);
}

void destroyMesh(mesh_t *mesh)
{
	uint i;
	for (i=0; i<mesh->nbFaces; i++)
	{
		destroyFace(mesh->faces[i]);
	}
	if (mesh->faces)
	{
		destroy(mesh->faces);
	}
	destroy(mesh);
}

void setAutoSelect(bool active)
{
	_autoSelect = active;
}

void selectMesh(mesh_t *mesh)
{
	_selectedMesh = mesh;
}

void selectFace(face_t *face)
{
	_selectedFace = face;
}

mesh_t *getSelectedMesh(void)
{
	return _selectedMesh;
}

face_t *getSelectedFace(void)
{
	return _selectedFace;
}

void _moveVertex(vertex_t *vertex, float vec[3])
{
	Vector.rotate(vec, _localAxis);
	vertex->coords.x += vec[0];
	vertex->coords.y += vec[1];
	vertex->coords.z += vec[2];
}

void moveVertex(vertex_t *vertex, float x, float y, float z)
{
	float vec[3];
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
	_moveVertex(vertex, vec);
}

void moveFace(face_t *face, float x, float y, float z)
{
	uint i;
	for (i=0; i<face->nbVertices; i++)
	{
		moveVertex(face->vertices[i], x, y, z);
	}
}

void moveMesh(mesh_t *mesh, float x, float y, float z)
{
	uint i;
	for (i=0; i<mesh->nbFaces; i++)
	{
		moveFace(mesh->faces[i], x, y, z);
	}
}

void _setCursorPos(float pos[3])
{
	_cursorPos[0] = pos[0];
	_cursorPos[1] = pos[1];
	_cursorPos[2] = pos[2];
}

void setCursorPos(float x, float y, float z)
{
	float pos[3];
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	_setCursorPos(pos);
}

void setCursorCoords(coords_t coords)
{
	setCursorPos(coords.x, coords.y, coords.z);
}

void moveCursorPos(float x, float y, float z)
{
	float pos[3];
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	Vector.rotate(pos, _localAxis);
	Vector.add(pos, _cursorPos, pos);
	_setCursorPos(pos);
}

void _setLocalAxis(float forward[3], float right[3], float up[3])
{
	int i;
	for (i=0; i<3; i++)
	{
		_localAxis[0][i] = forward[i];
		_localAxis[1][i] = right[i];
		_localAxis[2][i] = up[i];
	}
}

void setLocalAxis(float forward[3])
{
	float angles[3];
	float fwd[3], right[3], up[3];
	Vector.toAngles(forward, angles);
	Vector.axisFromAngles(angles, fwd, right, up);
	_setLocalAxis(fwd, right, up);
}

void setLocalAxisFromLine(float startX, float startY, float startZ, float endX, float endY, float endZ)
{
	float vec[3];
	vec[0] = endX - startX;
	vec[1] = endY - startY;
	vec[2] = endZ - startZ;
	Vector.normalize(vec, vec);
	setLocalAxis(vec);
}

void setLocalAxisFromCoords(coords_t start, coords_t end)
{
	setLocalAxisFromLine(start.x, start.y, start.z, end.x, end.y, end.z);
}

float getDistanceBetweenVertices(vertex_t *vert1, vertex_t *vert2)
{
	float pos1[3], pos2[3];

	pos1[0] = vert1->coords.x;
	pos1[1] = vert1->coords.y;
	pos1[2] = vert1->coords.z;

	pos2[0] = vert2->coords.x;
	pos2[1] = vert2->coords.y;
	pos2[2] = vert2->coords.z;

	return Vector.distance(pos1, pos2);
}

void resetWorkSpace(void)
{
	int i,j;
	_selectedMesh = NULL;
	_selectedFace = NULL;

	for (i=0; i<3; i++)
	{
		_cursorPos[i] = 0.0;
		for (j=0; j<3; j++)
		{
			_localAxis[i][j] = Vector.axis[i][j];
		}
	}

	_autoSelect = true;
}

mesh_t *fuseMeshes(mesh_t **a, mesh_t **b)
{
	mesh_t *mesh = newMesh();
	uint i;

	mesh->nbFaces = (*a)->nbFaces + (*b)->nbFaces;
	mesh->faces = newArray(face_t*, mesh->nbFaces);

	for (i = 0; i < (*a)->nbFaces; i++)
	{
		mesh->faces[i] = (*a)->faces[i];
	}
	for (i = 0; i < (*b)->nbFaces; i++)
	{
		mesh->faces[i + (*a)->nbFaces] = (*b)->faces[i];
	}

	Memory.free((*a)->faces);
	Memory.free((*b)->faces);
	Memory.free(*a);
	Memory.free(*b);
	*a = NULL;
	*b = NULL;

	return mesh;
}

void updateMeshGeometry(mesh_t *mesh)
{
	uint i, j;
#if 1
	for (i = 0; i < mesh->nbFaces; i++)
	{
		face_t *face = mesh->faces[i];

		face->vbo = newArray(float, face->nbVertices * 8);
		face->ebo = newArray(ushort, face->nbVertices);

		for (j = 0; j < face->nbVertices; j++)
		{
			((vertex_t*)face->vbo)[j] = *face->vertices[j];
			face->ebo[j] = (ushort)j;
		}

		glBindBuffer(GL_ARRAY_BUFFER, face->vboIndex);
		glBufferData(GL_ARRAY_BUFFER, 8 * face->nbVertices * sizeof(float), face->vbo, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face->eboIndex);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, face->nbVertices * sizeof(ushort), face->ebo, GL_STATIC_DRAW);
	}
#else
	uint nbVertices = 0;
	for (i = 0; i < mesh->nbFaces; i++)
	{
		nbVertices += mesh->faces[i]->nbVertices;
	}

	if (mesh->vbo)
	{
		mem_free(mesh->vbo);
	}

	// Not doing any index saving for now, also praying that every face is a triangle
	mesh->vbo = (float*)mem_alloc(sizeof(float) * nbVertices * 5);
	mesh->ebo = (ushort*)mem_alloc(sizeof(ushort) * nbVertices);

	nbVertices = 0;
	for (i = 0; i < mesh->nbFaces; i++)
	{
		for (j = 0; j < mesh->faces[i]->nbVertices; j++)
		{
			((vertex_t*)mesh->vbo)[nbVertices] = *mesh->faces[i]->vertices[j];
			mesh->ebo[nbVertices] = (ushort)nbVertices;
			nbVertices++;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vboIndex);
	glBufferData(GL_ARRAY_BUFFER, 5 * nbVertices * sizeof(float), mesh->vbo, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->eboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbVertices * sizeof(ushort), mesh->ebo, GL_DYNAMIC_DRAW);
#endif
}

void renderMesh(const mesh_t *mesh, float viewMatrix[16])
{ // Dirty way for now
	uint i;
	for (i = 0; i < mesh->nbFaces; i++)
	{
		face_t *face = mesh->faces[i];
		program_t *program = face->program;
		bool gotTexture = (face->texture != NULL);

		if (!program)
		{
			program = ProgramInternal->getDefault(gotTexture);
		}

		glUseProgram(program->programID);

		glUniform3fv(program->originLocation, 1, mesh->origin);
		glUniformMatrix4fv(program->rotationLocation, 1, GL_FALSE, mesh->rotation);

		if (gotTexture)
		{
			glActiveTexture(GL_TEXTURE0 + face->texture->textureID);
			glBindTexture(GL_TEXTURE_2D, face->texture->textureID);
			glUniform1i(program->textureLocation, face->texture->textureID);
		}
			
		glBindBuffer(GL_ARRAY_BUFFER, face->vboIndex);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face->eboIndex);

		glVertexAttribPointer(program->coordsLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
		glEnableVertexAttribArray(program->coordsLocation);
		glVertexAttribPointer(program->texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(program->texCoordsLocation);
		glVertexAttribPointer(program->normalsLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(program->normalsLocation);

		glUniformMatrix4fv(program->viewMatLocation, 1, GL_TRUE, viewMatrix);

		glUniform4fv(program->colorLocation, 1, face->color);

		glDrawElements(GL_TRIANGLES, face->nbVertices, GL_UNSIGNED_SHORT, NULL);

		if (gotTexture)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glUseProgram(0);
}

void initGeometryFunctions(_geometry_functions *Geometry)
{
	Geometry->makeSphere = createSphere;
	Geometry->makeRing = createRing;
	Geometry->addFace = addFace;
	Geometry->addFaceToMesh = addFaceToMesh;
	Geometry->addVertex = addVertex;
	Geometry->addVertexToFace = addVertexToFace;
	Geometry->fuseMeshes = fuseMeshes;
	Geometry->destroyFace = destroyFace;
	Geometry->destroyMesh = destroyMesh;
	Geometry->destroyVertex = destroyVertex;
	Geometry->duplicateFace = duplicateFace;
	Geometry->duplicateMesh = duplicateMesh;
	Geometry->duplicateVertex = duplicateVertex;
	Geometry->getDistanceBetweenVertices = getDistanceBetweenVertices;
	Geometry->getSelectedFace = getSelectedFace;
	Geometry->getSelectedMesh = getSelectedMesh;
	Geometry->linkFaceToMesh = linkFaceToMesh;
	Geometry->linkVertexToFace = linkVertexToFace;
	Geometry->moveCursorPos = moveCursorPos;
	Geometry->moveFace = moveFace;
	Geometry->moveMesh = moveMesh;
	Geometry->moveVertex = moveVertex;
	Geometry->newMesh = newMesh;
	Geometry->renderMesh = renderMesh;
	Geometry->resetWorkSpace = resetWorkSpace;
	Geometry->selectFace = selectFace;
	Geometry->selectMesh = selectMesh;
	Geometry->setAutoSelect = setAutoSelect;
	Geometry->setCursorCoords = setCursorCoords;
	Geometry->setCursorPos = setCursorPos;
	Geometry->setLocalAxis = setLocalAxis;
	Geometry->setLocalAxisFromCoords = setLocalAxisFromCoords;
	Geometry->setLocalAxisFromLine = setLocalAxisFromLine;
	Geometry->updateMeshGeometry = updateMeshGeometry;
}

#undef HASNOR_ENGINE_INTERNAL
