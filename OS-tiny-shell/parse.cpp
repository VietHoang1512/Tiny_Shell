#include <iostream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <Windows.h>

using namespace std;

#define MAX_BUFFER_SIZE 128
#define MAX_TOKENS 100
#define TOKEN_DELIM " "

char **SplitLine(char *buf)
{

	int pos = 0;
	char **tokens = (char **)calloc(MAX_TOKENS, sizeof(char *));
	char *token;

	if (tokens == NULL)
	{
		cout << " Memory allocation failed. \n";
		exit(EXIT_FAILURE);
	}
	token = strtok(buf, TOKEN_DELIM); // chia xau thanh cac token boi " ": Lay token dau tien
	while (token != NULL)
	{
		tokens[pos] = token;
		pos++;
		token = strtok(NULL, TOKEN_DELIM);
	}
	tokens[pos] = NULL;
	return tokens; // tra ve 1 chuoi con tro tro toi 1 mang cac string da duoc chia cua lenh
}
char *CombineLine(char **argv, int start)
{
	char *path = (char *)calloc(125, sizeof(char));
	char *pivot = (char *)" ";

	// Append argv[] and space char to path
	path = strcat(path, argv[start]);
	int i = start + 1;
	while (argv[i] != NULL)
	{
		path = strcat(path, pivot);
		path = strcat(path, argv[i]);
		++i;
	}
	return path;
}
