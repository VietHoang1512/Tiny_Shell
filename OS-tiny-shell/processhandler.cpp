#define WINVER 0x0500
#include <iostream>
#include <Windows.h>
#include <sys/types.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <string>
#include "parse.h"
#include <string.h>
#include <signal.h>
#include <iomanip>

using namespace std;

HANDLE h_fore_process = NULL;

BOOL GetProcessListAll()
{
	HANDLE h_process_snap;
	PROCESSENTRY32 pe32; // // 1 structure store the information of a thread
	// Take the snapshot of all the processes:
	h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h_process_snap == INVALID_HANDLE_VALUE)
	{
		cout << "CreateToolhelp32Snapshot failed\n";
		return FALSE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(h_process_snap, &pe32))
	{
		cout << "Process32First failed\n"; // show cause of failure
		CloseHandle(h_process_snap);	   // clean the snapshot object
		return FALSE;
	}
	// Retrieve the priority class.
	cout << setw(30) << left << "Process Name" << setw(20) << left << "Parent Process ID" << setw(10) << left << "Process ID\n";
	cout << "------------------------------------------------------------------------\n";
	do
	{
		cout << setw(30) << left << pe32.szExeFile << setw(20) << left << pe32.th32ParentProcessID << setw(10) << left << pe32.th32ProcessID << "\n";
	} while (Process32Next(h_process_snap, &pe32));
	CloseHandle(h_process_snap);
	return (TRUE);
}
BOOL GetChildProcessList(DWORD pid)
{
	HANDLE h_process_snap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h_process_snap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Fail %d\n", GetLastError());
		return FALSE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(h_process_snap, &pe32))
	{
		printf("Process32First Fail %d\n", GetLastError()); // show cause of failure
		CloseHandle(h_process_snap);          // clean the snapshot object
		return FALSE;
	}
	// Retrieve the priority class.
	printf("%-50s%-20s%-20s\n", "Process Name", "Process ID", "Parent Process ID");
	printf("%-50s%-20s%-20s\n", "----------------------------------", "----------", "-----------");
	do
	{
		if (pe32.th32ParentProcessID == pid)
			printf("%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
	} while (Process32Next(h_process_snap, &pe32));
	CloseHandle(h_process_snap);
	return(TRUE);
}

BOOL SuspendProcess(DWORD pid)
{
	// Take a snapshot of all threads in the process.
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 th32; // 1 structure store the information of a thread
	HANDLE h_thread;
	if (h_thread_snap == INVALID_HANDLE_VALUE)
	{
		cout << "CreateToolhelp32Snapshot failed\n";
		return FALSE;
	}
	// Set the size of the structure before using it.
	th32.dwSize = sizeof(THREADENTRY32);
	// Retrieve information about the first thread,
	if (!Thread32First(h_thread_snap, &th32))
	{
		cout << "Thread32First failed\n"; // show cause of failure
		CloseHandle(h_thread_snap);		  // clean the snapshot object
		return FALSE;
	}
	// Get all the threads of process and call the SuspendThread function to stop each one
	do
	{
		if (th32.th32OwnerProcessID == pid)
		{
			h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (SuspendThread(h_thread) == -1) //	DWORD SuspendThread(HANDLE hThread): return -1 if fails,else return previous suspend count of the thread
			{
				return FALSE;
			}
		}
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return TRUE;
}
BOOL ResumeProcess(DWORD pid)
{
	// Take a snapshot of all threads in the process.
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 th32;
	HANDLE h_thread;
	if (h_thread_snap == INVALID_HANDLE_VALUE)
	{
		cout << "CreateToolhelp32Snapshot failed\n";
		return FALSE;
	}
	// Set the size of the structure before using it.
	th32.dwSize = sizeof(THREADENTRY32);
	// Retrieve information about the first thread,
	if (!Thread32First(h_thread_snap, &th32))
	{
		cout << "Thread32First failed\n";
		CloseHandle(h_thread_snap);
		return FALSE;
	}
	// get all the threads of process and resume each one
	do
	{
		if (th32.th32OwnerProcessID == pid)
		{
			h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, th32.th32ThreadID);
			if (ResumeThread(h_thread) == -1) // DWORD ResumeThread(HANDLE h_thread): decrease suspend's count. if (suspend count == 0), thread is resume.
											  // Function return -1 if fails, else return previous suspend count of the thread.
			{
				return FALSE;
			}
		}
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return TRUE;
}

BOOL KillProcess(DWORD pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); // open a process by its pid
	if (hProcess == NULL)
	{
		cout << "Failed!\n";
		return EXIT_FAILURE;
	}
	if (TerminateProcess(hProcess, 1) == 0) // BOOL TerminateProcess(HANDLE hProcess, UINT   uExitCode: return 0 if fails
	{
		cout << "Failed!\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void handleSigint(int sigInt)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, sigInt);
	TerminateProcess(hProcess, 1);
	CloseHandle(hProcess);
	return;
}

BOOL CreateNewProcess(char **argv)
{
	int wait_time;
	if (argv[2] == NULL) // Command ps -b, -f  has at least 3 arguments.
	{
		cout << "Too few argument\n";
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "-back"))
	{
		wait_time = INFINITE;
	}
	else
	{
		wait_time = 0;
	}

	char *run_file = CombineLine(argv, 2);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.lpTitle = argv[1];
	ZeroMemory(&pi, sizeof(pi));

	// Start the child process.
	if (!CreateProcess(NULL,			   // No module name (use command line)
					   run_file,		   // Command line
					   NULL,			   // Process handle not inheritable
					   NULL,			   // Thread handle not inheritable
					   FALSE,			   // Set handle inheritance to FALSE
					   CREATE_NEW_CONSOLE, // Create new console
					   NULL,			   // Use parent's environment block
					   NULL,			   // Use parent's starting directory
					   &si,				   // Pointer to STARTUPINFO structure
					   &pi)				   // Pointer to PROCESS_INFORMATION structure
	)
	{
		cout << "Create process failed\n";
		return EXIT_FAILURE;
	}

	// get handle of child fore process
	if (strcmp(argv[1], "-fore") == 0)
	{
		h_fore_process = pi.hProcess;
		signal(SIGINT, handleSigint);
	}
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, wait_time);

	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return EXIT_SUCCESS;
}
BOOL GetThreadList(DWORD pid)
{
	// Take a snapshot of all threads in the process.
	HANDLE h_thread_snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	THREADENTRY32 th32;
	if (h_thread_snap == INVALID_HANDLE_VALUE)
	{
		printf("CreateToolhelp32Snapshot Fail %d\n", GetLastError());
		return FALSE;
	}
	// Set the size of the structure before using it.
	th32.dwSize = sizeof(THREADENTRY32);
	// Retrieve information about the first thread,
	if (!Thread32First(h_thread_snap, &th32))
	{
		printf("Thread32First Fail %d\n", GetLastError()); // show cause of failure
		CloseHandle(h_thread_snap);          // clean the snapshot object
		return FALSE;
	}
	// Walk other threads
	printf("%-50s%-20s\n", "Thread ID", "OwnerProcessID");
	printf("%-50s%-20s\n", "----------------------------------", "----------");
	do
	{
		if (th32.th32OwnerProcessID == pid)
			printf("%-50d%-20d\n", th32.th32ThreadID, th32.th32OwnerProcessID);
	} while (Thread32Next(h_thread_snap, &th32));
	CloseHandle(h_thread_snap);
	return TRUE;
}
BOOL FindProcessID(const char *process_name)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return(EXIT_FAILURE);

	pe32.dwSize = sizeof(PROCESSENTRY32); // <----- IMPORTANT

	// Retrieve information about the first process,
	// and exit if unsuccessful
	printf("%-50s%-20s%-20s\n", "Process Name", "Process ID", "Parent Process ID");
	printf("%-50s%-20s%-20s\n", "----------------------------------", "----------", "-----------");
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		printf("!!! Failed to gather information on system processes! \n");
		return EXIT_FAILURE;
	}

	do
	{
		// printf("Checking process %ls\n", pe32.szExeFile);
		if (0 == strcmp(process_name, pe32.szExeFile))
		{
			printf("%-50s%-20d%-20d\n", pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return EXIT_SUCCESS;
}
