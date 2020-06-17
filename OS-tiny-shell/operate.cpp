#include <iostream>
#include <Windows.h>
#include "built_in_cmd.h"
#include "processhandler.h"

const char *spe_arg[] = {"help", "cd", "exit", "enva", "date", "time", "dir", "ps"};

BOOL ProcessCommand(char **argv)
{
	if (argv[1] == NULL) // Lenh ps co it nhat 2 doi so
	{
		cout << "Too few arguments\n";
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-all") == 0)
	{
		// Print All Process
		if (GetProcessListAll())
		{
			return EXIT_SUCCESS;
		}
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-suspend") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "Too few arguments\n";
			return EXIT_FAILURE;
		}
		// Suspend Process
		DWORD pid = atoi(argv[2]);
		if (SuspendProcess(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-resume") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "Too few arguments\n";
			return EXIT_FAILURE;
		}
		// Resume Process
		DWORD pid = atoi(argv[2]);
		if (ResumeProcess(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "-fore") == 0 || strcmp(argv[1], "-back") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "Too few arguments\n";
			return EXIT_FAILURE;
		}
		// Find a PID of Process
		if (CreateNewProcess(argv))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-kill") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "Too few arguments\n";
			return EXIT_FAILURE;
		}
		// Suspend Process
		DWORD pid = atoi(argv[2]);
		if (KillProcess(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	cout << "Command " << argv[0] << " does not have " << argv[1] << " option \n";
	return EXIT_FAILURE;
}

BOOL BuiltInCommand(char **argv, LPSTR cur_dir)
{
	if (strcmp(argv[0], spe_arg[0]) == 0) // Neu doi so truyen vao = "help" => Thuc hien lenh Help
	{
		HelpTask(argv);
	}
	else if (strcmp(argv[0], spe_arg[1]) == 0) // Neu doi so truyen vao = "help" => Thuc hien lenh cd, voi thu muc hien tai: cur_dir
	{
		CDTask(argv, cur_dir);
	}
	else if (strcmp(argv[0], spe_arg[2]) == 0) // Neu doi so truyen vao = "exit" => Ket thuc shell
	{
		exit(0);
	}
	else if (strcmp(argv[0], spe_arg[3]) == 0) // Neu doi so truyen vao = "enva"...
	{
		AnalysisEnvironmentVar(argv);
	}
	else if (strcmp(argv[0], spe_arg[4]) == 0)
	{
		DateTask(argv);
	}
	else if (strcmp(argv[0], spe_arg[5]) == 0)
	{
		TimeTask(argv);
	}
	else if (strcmp(argv[0], spe_arg[6]) == 0)
	{
		DirTask(argv, cur_dir);
	}
	else if (strcmp(argv[0], spe_arg[7]) == 0)
	{
		ProcessCommand(argv);
	}
	else
	{
		cout << "Command not found.\nType 'help' to know more information\n";
		return 1;
	}
	return 0;
}