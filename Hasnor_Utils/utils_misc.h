#ifndef MISC_UTILS_H_DEFINED
#define MISC_UTILS_H_DEFINED

/*
utils_misc

- Utility functions that don't belong anywhere else
*/

char *quickString(const char *s);
char *quickString2(const char *s, unsigned int len);

char *strFromVec(float vec[3]);
char *strFromInt(int n);
char *strFromFloat(float n);
void strip(char *s, char c);

int parseInt(char *s);
float parseFloat(char *s);

int isInRange(float min, float val, float max);
unsigned int nbDigits(int n);

int randomIntBetween(int min, int max);
float randomValueBetween(float min, float max);

float atLeastOne(float x);
void strcpy_safe(char *dst, char *src);

bool charsEqualCaseInsensitive(char a, char b);

void quit(void);

#endif