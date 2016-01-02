#ifndef HASNOR_STRING_DEFINED
#define HASNOR_STRING_DEFINED

#include "utils_types.h"

/*typedef struct {
	char			*s;
	uint			len;
} string;*/

typedef struct {
	/* Initialization */
	char*		(*create)(const char *value);
	char*		(*createCapped)(const char *value, uint maxLength);
	char*		(*fromInt)(int value);
	char*		(*fromFloat)(float value);
	char*		(*fromVector)(const float *vec);
	//char*		(*fromFormat)(const char *str, ...); // TODO
	//string*		(*precompute)(const char *value); // This would only be a good idea if a lot of data was precomputed, length isn't enough

	/* Modification */
	void		(*copy)(char *dest, const char *src);
	void		(*copySafe)(char *dest, const char *src);
	void		(*strip)(char *str, char c);
	void		(*append)(char **str, const char *value);
	void		(*replace)(char **str, const char *pattern, const char *value);
	void		(*insert)(char **str, const char *value, uint pos);

	/* Extraction */
	uint		(*length)(const char *str);
	char*		(*substring)(const char *str, uint start, uint end);
	bool		(*findChar)(const char *str, char c, uint *index);
	bool		(*findString)(const char *str, const char *pattern, uint *index);
	bool		(*compare)(const char *str1, const char *str2, bool caseSensitive);
	int			(*parseInt)(const char *str);
	float		(*parseFloat)(const char *str);
	void		(*parseVector)(const char *str, float *out);
} _string_functions;

_string_functions String;

#ifdef HASNOR_INIT
void initStringFunctions(void);
#endif

#endif