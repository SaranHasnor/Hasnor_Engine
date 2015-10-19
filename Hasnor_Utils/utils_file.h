#ifndef HASNOR_FILE_IMPORTED
#define HASNOR_FILE_IMPORTED

#include "utils_types.h"

typedef struct {
	bool (*read)(const char *path, char **out);
} _file_functions;

_file_functions File;

#ifdef HASNOR_INIT
void initFileFunctions();
#endif

#endif