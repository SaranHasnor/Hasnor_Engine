#include "engine_init.h"

#define HASNOR_INIT
#include "engine.h"
#include "engine_camera.h"
#include "engine_texture.h"
#include "engine_shader.h"
#include "engine_mesh.h"
#include "engine_geometry.h"
#include "engine_particles.h"
#include "engine_interface.h"
#include "engine_window.h"
#undef HASNOR_INIT

void initHasnorEngine(void)
{
	initEngineFunctions();
	
	initTextureFunctions();
	initShaderFunctions();
	initProgramFunctions();

	initMeshFunctions();
	initGeometryFunctions();

	initParticleFunctions();

	initCameraFunctions(&Engine.Camera);
	initInterfaceFunctions(&Engine.UI);
	initWindowFunctions(&Engine.Window);
}