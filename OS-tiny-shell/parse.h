#ifndef _PARSE_H
#define _PARSE_H

#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <Windows.h>
#include <string>

// Read input
char *ReadLine(void);

// Split input to many char *
char **SplitLine(char *buf);

// Combine path to a string
char *CombineLine(char **argv, int start);

#endif
