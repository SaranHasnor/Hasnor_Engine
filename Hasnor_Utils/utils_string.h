#ifndef HASNOR_STRING_DEFINED
#define HASNOR_STRING_DEFINED

#include "utils_types.h"

/*
utils_string

- Functions and structures for creating and parsing arrays of characters
*/

typedef struct {
	char			*s;
	uint			len;
} string;

typedef struct {
	/* Initialization */
	string*		(*create)(const char *value);
	void		(*init)(string *str, const char *value, uint optionalSize);
	void		(*fromInt)(string *str, int value);
	void		(*fromFloat)(string *str, float value);
	void		(*fromVector)(string *str, const float *vec, uint size);
	//void		(*fromFormat)(string *str, ...); // TODO
	string*		(*duplicate)(const string *str);

	/* Modification */
	void		(*strip)(string *str, char c);
	void		(*append)(string *str, const char *value);
	void		(*replace)(string *str, const char *pattern, const char *value);
	void		(*insert)(string *str, const char *value, uint pos);

	/* Extraction */
	uint		(*length)(const char *str);
	string*		(*substring)(const string *str, uint start, uint end);
	bool		(*findChar)(const string *str, char c, uint *index);
	bool		(*findString)(const string *str, const char *pattern, uint *index);
	bool		(*compare)(const string *str, const char *pattern, bool caseSensitive);
	int			(*parseInt)(const string *str);
	float		(*parseFloat)(const string *str);
} _string_functions;

_string_functions String;

#ifdef HASNOR_INIT
void initStringFunctions(void);
#endif

#endif