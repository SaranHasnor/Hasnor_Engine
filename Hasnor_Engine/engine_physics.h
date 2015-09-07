

typedef struct {
	float		position[3];
	float		angles[3];

	float		velocity[3];
	float		rotation[3];
} transform_t;

void updateTransform(transform_t *transform, float deltaTime);