#pragma once

#ifndef _PROCESSHANDLER_H_
#define _PROCESSHANDLER_H_

#include <iostream>
#include <Windows.h>
#include <sys/types.h>
using namespace std;

// Print All Process on System
BOOL GetProcessListAll();

// Print All Child of Process which has PID
BOOL GetChildProcessList(DWORD pid);

// Print All Thread of Process which has PID
BOOL GetThreadList(DWORD ProcessID);

// Suspend Process which has PID
BOOL SuspendProcess(DWORD pid);

// Resume Process which has PID
BOOL ResumeProcess(DWORD pid);

// Find Process ID by its name
BOOL FindProcessID(const char *process_name);

// Kill Process by ID
BOOL KillProcess(DWORD pid);

// Create a new process
BOOL CreateNewProcess(char **argv);

#endif