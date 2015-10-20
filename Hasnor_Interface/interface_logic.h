#include "interface_utils.h"

#ifdef HASNOR_ENGINE_INTERNAL
void interface_init(void);
bool interface_clickDown(int x, int y);
void interface_clickHeld(int x, int y);
void interface_clickUp(int x, int y);
bool interface_keyPressed(unsigned char key, int x, int y);
void interface_update(float deltaTime);
void interface_render(void);
void interface_reshapeWindow(int width, int height);
#endif
