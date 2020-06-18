#pragma once

#ifndef _OPERATE_H_
#define _OPERATE_H_

#include <iostream>
#include <Windows.h>
#include <sys/types.h>
using namespace std;

BOOL BuiltInCommand(char **argv, LPSTR cur_dir);

BOOL ProcessCommand(char **argv);

#endif