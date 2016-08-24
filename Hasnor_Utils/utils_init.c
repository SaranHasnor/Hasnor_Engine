#include "utils_init.h"

#define HASNOR_INIT
#include "utils_array.h"
#include "utils_bytestream.h"
#include "utils_console.h"
#include "utils_debug.h"
#include "utils_file.h"
#include "utils_list.h"
#include "utils_map.h"
#include "utils_math.h"
#include "utils_matrix.h"
#include "utils_memory.h"
#include "utils_object.h"
#include "utils_program.h"
#include "utils_quaternion.h"
#include "utils_string.h"
#include "utils_system.h"
#include "utils_thread.h"
#include "utils_time.h"
#include "utils_vector.h"
#undef HASNOR_INIT

void initHasnorUtils(void)
{
	initArrayFunctions();
	initByteStreamFunctions();
	initConsoleFunctions();
	initDebugFunctions();
	initFileFunctions();
	initListFunctions();
	initMapFunctions();
	initMathFunctions();
	initMatrixFunctions();
	initMemoryFunctions();
	initObjectFunctions();
	initProgramFunctions();
	initQuaternionFunctions();
	initStringFunctions();
	initSystemFunctions();
	initThreadFunctions();
	initTimeFunctions();
	initVectorFunctions();
}