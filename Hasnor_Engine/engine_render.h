#ifndef ENGINE_RENDER_IMPORTED
#define ENGINE_RENDER_IMPORTED

#include <utils.h>
#include <utils_vector.h>

#include "engine_texture.h"

typedef struct {
	float u;
	float v;
} texCoords_t;

typedef struct {
	float x;
	float y;
	float z;
} coords_t;

typedef struct {
	coords_t	coords;
	texCoords_t texCoords;
} vertex_t;

typedef struct {
	vertex_t	**vertices;
	uint		nbVertices;

	float		*vbo;
	uint		vboIndex;
	ushort		*ebo;
	uint		eboIndex;

	program_t	*program;
	texture_t	*texture;

	float		color[4];
} face_t;

typedef struct {
	face_t		**faces;
	uint		nbFaces;

	float		origin[3];
	float		rotation[16];

	/*float		*vbo;
	uint		vboIndex;
	ushort		*ebo;
	uint		eboIndex;

	uint		nbVertices;*/
} mesh_t;

// Construction functions
vertex_t *addVertexToFace(face_t *face, float x, float y, float z, float u, float v);
face_t *addFaceToMesh(mesh_t *mesh);
mesh_t *newMesh();

// Linking function
void linkVertexToFace(face_t *face, vertex_t *vertex);
void linkFaceToMesh(mesh_t *mesh, face_t *face);

// Quick construction functions (using selected objects)
vertex_t *addVertex(float x, float y, float z, float u, float v);
face_t *addFace();

// Destruction functions
void destroyVertex(vertex_t *vertex);
void destroyFace(face_t *face);
void destroyMesh(mesh_t *mesh);

// Transformation functions
void moveVertex(vertex_t *vertex, float x, float y, float z);
void moveFace(face_t *face, float x, float y, float z);
void moveMesh(mesh_t *mesh, float x, float y, float z);

//void rotateFace(face_t *face, float angle, float x, float y, float z);
//void rotateMesh(mesh_t *mesh, float angle, float x, float y, float z);

//void scaleFace(face_t *face, float x, float y, float z);
//void scaleMesh(mesh_t *mesh, float x, float y, float z);

// Duplication functions (unsafe for now)
vertex_t *duplicateVertex(vertex_t *vertex);
face_t *duplicateFace(face_t *face);
mesh_t *duplicateMesh(mesh_t *mesh);

// Selection management functions
void setAutoSelect(bool active);
void selectFace(face_t *face);
void selectMesh(mesh_t *mesh);
face_t *getSelectedFace();
mesh_t *getSelectedMesh();

// Cursor management functions
void setCursorPos(float x, float y, float z);
void setCursorCoords(coords_t coords);
void moveCursorPos(float x, float y, float z);

// Local Axis Management functions
void setLocalAxis(float forward[3]);
void setLocalAxisFromLine(float startX, float startY, float startZ, float endX, float endY, float endZ);
void setLocalAxisFromCoords(coords_t start, coords_t end);

// Utility functions
float getDistanceBetweenVertices(vertex_t *vert1, vertex_t *vert2);
void resetWorkSpace();

// Rendering functions
void updateMeshGeometry(mesh_t *mesh);
void renderMesh(const mesh_t *mesh, float viewMatrix[16]);

#endif