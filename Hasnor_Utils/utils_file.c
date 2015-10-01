#include "utils_file.h"
#include "utils_ctools.h"

#pragma warning (disable:4996)

bool file_read(const char *path, char **out)
{
	FILE *file = fopen(path, "r");
	long size;

	if (!file)
	{
		return false;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	*out = newArray(char, size);

	fseek(file, 0, SEEK_SET);
	fread(*out, sizeof(char), size, file);

	fclose(file);

	return true;
}

void initFileFunctions()
{
	File.read = file_read;
}