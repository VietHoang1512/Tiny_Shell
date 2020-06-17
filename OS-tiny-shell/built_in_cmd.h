#pragma once
#ifndef _BUILT_IN_CMD_H
#define _BUILT_IN_CMD_H

#include <iostream>
#include <Windows.h>
#include <ctime>
#include "parse.h"

using namespace std;

BOOL HelpTask(char **argv);

BOOL CDTask(char **argv, LPSTR cur_dir);

BOOL DateTask(char **argv);

BOOL TimeTask(char **argv);

BOOL DirTask(char **argv, LPSTR cur_dir);

BOOL GetAllEnvironmentVar();

BOOL GetSpecificEnvironmentVar(char **argv);

BOOL SetEnvironmentVar(char **argv);

BOOL AnalysisEnvironmentVar(char **argv);
// Analysis input to choose the function to execute

#endif
