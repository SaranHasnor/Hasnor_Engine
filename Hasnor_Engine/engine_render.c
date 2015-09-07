#include "engine_render.h"
#include <utils_matrix.h>
#include <utils_quaternion.h>

#include <GL/glew.h>

/*
engine_render

- 3D mesh generation and manipulation functions
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

	if (face->nbVertices == 0)
	{
		face->vertices = (vertex_t**)mem_alloc(sizeof(vertex_t*));
	}
	else
	{
		face->vertices = (vertex_t**)mem_realloc(face->vertices, sizeof(vertex_t*) * (face->nbVertices+1));
	}

	face->vertices[face->nbVertices++] = vertex;
}

vertex_t *_addVertexToFace(face_t *face, float pos[3], float u, float v)
{
	vertex_t *vertex = (vertex_t*)mem_alloc(sizeof(vertex_t));
	vectorRotate(pos, _localAxis);
	vertex->coords.x = pos[0] + _cursorPos[0];
	vertex->coords.y = pos[1] + _cursorPos[1];
	vertex->coords.z = pos[2] + _cursorPos[2];
	vertex->texCoords.u = u;
	vertex->texCoords.v = v;
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

	if (mesh->nbFaces == 0)
	{
		mesh->faces = (face_t**)mem_alloc(sizeof(face_t*));
	}
	else
	{
		mesh->faces = (face_t**)mem_realloc(mesh->faces, sizeof(face_t*) * (mesh->nbFaces+1));
	}

	mesh->faces[mesh->nbFaces++] = face;
}

face_t *addFaceToMesh(mesh_t *mesh)
{
	face_t *face = (face_t*)mem_alloc(sizeof(face_t));
	mem_set(face, 0, sizeof(face_t));

	glGenBuffers(1, &face->vboIndex);
	glGenBuffers(1, &face->eboIndex);

	vectorSet(face->color, 1.0f, 1.0f, 1.0f);
	face->color[3] = 1.0f;

	linkFaceToMesh(mesh, face);

	if (_autoSelect) _selectedFace = face;

	return face;
}

face_t *addFace()
{ // Adds a face to the currently selected mesh
	if (_selectedMesh)
	{
		return addFaceToMesh(_selectedMesh);
	}
	return NULL;
}

mesh_t *newMesh()
{
	mesh_t *mesh = (mesh_t*)mem_alloc(sizeof(mesh_t));
	mem_set(mesh, 0, sizeof(mesh_t));

	//glGenBuffers(1, &mesh->vboIndex);
	//glGenBuffers(1, &mesh->eboIndex);

	mat_identity(mesh->rotation);

	if (_autoSelect) _selectedMesh = mesh;

	return mesh;
}

vertex_t *duplicateVertex(vertex_t *vertex)
{
	vertex_t *newVertex = (vertex_t*)mem_dupe(vertex, sizeof(vertex_t));
	return newVertex;
}

face_t *duplicateFace(face_t *face)
{
	face_t *newFace = (face_t*)mem_alloc(sizeof(face_t));
	uint i;

	newFace->nbVertices = face->nbVertices;
	newFace->vertices = (vertex_t**)mem_alloc(sizeof(vertex_t*)*newFace->nbVertices);

	for (i=0; i<newFace->nbVertices; i++)
	{
		newFace->vertices[i] = duplicateVertex(face->vertices[i]);
	}

	return newFace;
}

mesh_t *duplicateMesh(mesh_t *mesh)
{
	mesh_t *newMesh = (mesh_t*)mem_alloc(sizeof(mesh_t));
	uint i;

	newMesh->nbFaces = mesh->nbFaces;
	newMesh->faces = (face_t**)mem_alloc(sizeof(face_t*)*newMesh->nbFaces);

	for (i=0; i<newMesh->nbFaces; i++)
	{
		newMesh->faces[i] = duplicateFace(mesh->faces[i]);
	}

	return newMesh;
}

void destroyVertex(vertex_t *vertex)
{
	mem_free(vertex);
}

void destroyFace(face_t *face)
{
	uint i;
	for (i=0; i<face->nbVertices; i++)
	{
		destroyVertex(face->vertices[i]);
	}
	mem_free(face->vertices);
	mem_free(face);
}

void destroyMesh(mesh_t *mesh)
{
	uint i;
	for (i=0; i<mesh->nbFaces; i++)
	{
		destroyFace(mesh->faces[i]);
	}
	mem_free(mesh->faces);
	mem_free(mesh);
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

mesh_t *getSelectedMesh()
{
	return _selectedMesh;
}

face_t *getSelectedFace()
{
	return _selectedFace;
}

void _moveVertex(vertex_t *vertex, float vec[3])
{
	vectorRotate(vec, _localAxis);
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
	vectorRotate(pos, _localAxis);
	vectorAdd(pos, _cursorPos, pos);
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
	vectoangles(forward, angles);
	AngleVectors(angles, fwd, right, up);
	_setLocalAxis(fwd, right, up);
}

void setLocalAxisFromLine(float startX, float startY, float startZ, float endX, float endY, float endZ)
{
	float vec[3];
	vec[0] = endX - startX;
	vec[1] = endY - startY;
	vec[2] = endZ - startZ;
	vectorNormalize(vec);
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

	return vectorDistance(pos1, pos2);
}

void resetWorkSpace()
{
	int i,j;
	_selectedMesh = NULL;
	_selectedFace = NULL;

	for (i=0; i<3; i++)
	{
		_cursorPos[i] = 0.0;
		for (j=0; j<3; j++)
		{
			_localAxis[i][j] = axis[i][j];
		}
	}

	_autoSelect = true;
}

void updateMeshGeometry(mesh_t *mesh)
{
	uint i, j;
#if 1
	for (i = 0; i < mesh->nbFaces; i++)
	{
		face_t *face = mesh->faces[i];

		face->vbo = (float*)mem_alloc(sizeof(float) * face->nbVertices * 5);
		face->ebo = (ushort*)mem_alloc(sizeof(ushort) * face->nbVertices);

		for (j = 0; j < face->nbVertices; j++)
		{
			((vertex_t*)face->vbo)[j] = *face->vertices[j];
			face->ebo[j] = (ushort)j;
		}

		glBindBuffer(GL_ARRAY_BUFFER, face->vboIndex);
		glBufferData(GL_ARRAY_BUFFER, 5 * face->nbVertices * sizeof(float), face->vbo, GL_STATIC_DRAW);

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
			program = defaultProgram(gotTexture);
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

		glVertexAttribPointer(program->coordsLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
		glEnableVertexAttribArray(program->coordsLocation);
		glVertexAttribPointer(program->texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(program->texCoordsLocation);

		glUniformMatrix4fv(program->viewMatLocation, 1, GL_TRUE, viewMatrix);

		glUniform4fv(program->colorLocation, 1, face->color);

		glDrawElements(GL_TRIANGLES, face->nbVertices, GL_UNSIGNED_SHORT, NULL);
	}
	glUseProgram(0);
}