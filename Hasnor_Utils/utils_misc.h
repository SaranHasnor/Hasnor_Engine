#ifndef MISC_UTILS_H_DEFINED
#define MISC_UTILS_H_DEFINED

/*
utils_misc

- Utility functions that don't belong anywhere else
*/

char *quickString(const char *s);
char *quickString2(const char *s, unsigned int len);

char *strFromVec(const float vec[3]);
char *strFromInt(int n);
char *strFromFloat(float n);

unsigned int nbDigits(int n);

void strcpy_safe(char *dst, char *src);

bool charsEqualCaseInsensitive(char a, char b);

void quit(void);

#endif