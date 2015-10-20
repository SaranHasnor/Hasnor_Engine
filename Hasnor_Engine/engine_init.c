#include "engine_init.h"

#define HASNOR_INIT
#include "engine.h"
#include "engine_camera.h"
#include "engine_texture.h"
#include "engine_geometry.h"
#include "engine_particles.h"
#include "engine_interface.h"
#include "engine_window.h"
#undef HASNOR_INIT

void initHasnorEngine(void)
{
	initEngineFunctions();
	
	initCameraFunctions(&Engine.Camera);
	initTextureFunctions(&Engine.Render.Texture);
	initShaderFunctions(&Engine.Render.Shader);
	initProgramFunctions(&Engine.Render.Program);
	initGeometryFunctions(&Engine.Render.Geometry);
	initParticleFunctions(&Engine.Render.Particles);
	initInterfaceFunctions(&Engine.UI);
	initWindowFunctions(&Engine.Window);
}