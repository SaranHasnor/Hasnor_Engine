#include "utils_file.h"
#include "utils_ctools.h"

#pragma warning (disable:4996)

bool file_read(const char *path, char **out)
{
	FILE *file = fopen(path, "r");
	ulong size;

	if (!file)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	size = (ulong)ftell(file);
	*out = newArray(char, size+1);

	fseek(file, 0, SEEK_SET);
	size = fread(*out, sizeof(char), size, file);
	(*out)[size] = '\0';

	fclose(file);

	return true;
}

void initFileFunctions(void)
{
	File.read = file_read;
}