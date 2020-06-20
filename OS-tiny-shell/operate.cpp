#include <iostream>
#include <Windows.h>
#include "built_in_cmd.h"
#include "processhandler.h"

const char *spe_non_arg[] = {"exit"};
const char *spe_one_arg[] = {"help", "date", "time", "enva", "ps"};
const char *spe_two_arg[] = {"cd", "dir"};
BOOL (*builtin_one_param)[](**args) = {
	&HelpTask;
	&DateTask;
	&TimeTask;
	&AnalysisEnvironmentVar;
	&ProcessCommand;
};

BOOL (*builtin_two_param)[](**args, LPSTR cur_dir) = {
	&CDTask;
	&DirTask;
};

BOOL ProcessCommand(char **argv)
{
	if (argv[1] == NULL) // Lenh ps co it nhat 2 doi so
	{
		cout << "At least 1 argument required\n";
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
	if (strcmp(argv[1], "-thread") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "At least 1 argument required\n";
			return EXIT_FAILURE;
		}
		// Get List Thread of Process
		DWORD pid = atoi(argv[2]);
		if (GetThreadList(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-find") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "At least 1 argument required\n";
			return EXIT_FAILURE;
		}
		// Find a PID of Process
		if (FindProcessID(argv[2]))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-child") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "At least 1 argument required\n";
			return EXIT_FAILURE;
		}
		// Print All Child Process
		DWORD pid = atoi(argv[2]);
		if (GetChildProcessList(pid))
			return EXIT_SUCCESS;
		else
			return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-suspend") == 0)
	{
		if (argv[2] == NULL)
		{
			cout << "At least 1 argument required\n";
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
			cout << "At least 1 argument required\n";
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
			cout << "At least 1 argument required\n";
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
			cout << "At least 1 argument required\n";
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

BOOL BuiltInCommand(char **argv, LPSTR cur_dir){
	int i;
	bool done = false ;
	if (strcmp(argv[0], spe_non_arg[0])==0){
		done = true;
		exit(0);
	}
	for(i=0; i<5;++i){
		if(strcmp(argv[0],spe_one_arg[i])==0){
			done =  true;
			(*builtin_one_param[i])(argv);
		}
	}
	for(i=0; i<2;++i){
		if(strcmp(argv[0],spe_two_arg[i])==0){
			done =  true;
			(*builtin_two_param[i])(argv, cur_dir));
		}
	}
	if(!done){
		cout << "Command not found.\nType 'help' to know more information\n";
		return 1;
	}
	return 0; 
}
