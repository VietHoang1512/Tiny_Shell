#include <iostream>
#include <Windows.h>
#include "parse.h"
#include <ctime>
#include "processhandler.h"
#include <vector>
#include <iomanip>

using namespace std;
//const char *spe_arg[] = { "help", "cd", "exit", "enva" ,"date", "time", "dir" , "ps"};

BOOL HelpTask(char **argv)
{
	if (argv[2] != NULL)
	{
		cout << "Provides help information for TinyShell command" << endl;
		cout << "Syntax :\n";
		cout << "       "
			 << "help -[command]" << endl;
		return EXIT_FAILURE;
	}
	if (argv[1] == NULL)
	{
		cout << "Type \"help -[command]\" to understand the command obviously.\n";
		cout << "Suppoted commands:\n cd\n date\n time\n dir\n ps\n enva\n exit\n";
	}
	else if (!strcmp(argv[1], "-cd"))
	{
		cout << "Change the current directory. You must write the new directory after this command.\nEXAMPLE: \"cd C:/\"\n";
	}
	else if (!strcmp(argv[1], "-time"))
	{
		cout << "Show the current time.\n";
	}
	else if (!strcmp(argv[1], "-date"))
	{
		cout << " Show the current date.\n";
	}
	else if (!strcmp(argv[1], "-ps"))
	{
		cout << "Supported options:\n";
		cout << " " << setw(15) << left << "all"
			 << "Print all the running processes.\n";
		cout << " " << setw(15) << left << "back"
			 << "Running a process in background mode.\n";
		cout << " " << setw(15) << left << "fore"
			 << "Running a process in foreground mode.\n";
		cout << " " << setw(15) << left << "suspend"
			 << "Suspend a process by its pid.\n";
		cout << " " << setw(15) << left << "resume"
			 << "Resume a process by its pid.\n";
		cout << " " << setw(15) << left << "kill"
			 << "Terminate a process by its pid.\n";
		cout << " " << setw(15) << left << "child"
			<< "Show all the childrent process of a process by its pid\n";
		cout << " " << setw(15) << left << "thread"
			<< "Show all the threads of a process by its pid\n";
		cout << " " << setw(15) << left << "find"
			<< "Find a process by its pid\n";
		cout << "Systex parameter: " << endl;
		cout << "                "
			 << "-[option]" << endl;
	}
	else if (!strcmp(argv[1], "-dir"))
	{
		cout << "Show list of files or folders in current folder or specific folder (write path after this command).\n";
		cout << "EXAMPLES: \"dir\", \"dir C:/\"\n";
	}
	else if (!strcmp(argv[1], "-enva"))
	{
		cout << "Supported options:\n";
		cout << " " << setw(15) << left << "all"
			 << "Show all the environment variables.\n";
		cout << " " << setw(15) << left << "get"
			 << "Get value of a specific environment variable.\n";
		cout << " " << setw(15) << left << "set"
			 << "Set value of a specific environment variable.\n";
	}
	else if (!strcmp(argv[1], "-exit"))
	{
		cout << "Exit shell.\n";
	}
	else
	{
		cout << "Command not found!\nType 'help' to know more details.\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

BOOL CDTask(char **argv, LPSTR cur_dir)
{
	if (argv[1] == NULL) // Lenh cd can 2 doi so
	{
		cout << "Current Directory: " << cur_dir << endl;
		return EXIT_FAILURE;
	}
	char *path = CombineLine(argv, 1);
	if (SetCurrentDirectory(path) == 0) // Neu duong dan khong dung:
	{
		cout << "The system cannot find the specified path.\n";
		return EXIT_FAILURE;
	}
	else
	{
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}

BOOL DateTask(char **argv)
{
	if (argv[1] != NULL)
	{
		cout << "Command \"date\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);
	struct tm *now = localtime(&t);
	cout << "Current Date: " << now->tm_mday << '/'
		 << (now->tm_mon + 1) << '/'
		 << (now->tm_year + 1900)
		 << endl;
	return true;
}

BOOL TimeTask(char **argv) // Lay thoi gian he thong
{
	if (argv[1] != NULL) // Khong co tham so truyen vao
	{
		cout << "Command \"time\" does not support any option !\n";
		return EXIT_FAILURE;
	}
	time_t t = time(0);				// tra ve thoi gian hien tai tinh theo thoi gian he thong (bat dau tu 1/1/1970) (tinh theo s)
	struct tm *now = localtime(&t); // lay cau truc tm tra ve thoi gian hien tai	(tm.year bat dau tu 1900, tm.month bat dau tu thang 0)
	cout << "Current time: " << now->tm_hour << ':' << now->tm_min << ':' << now->tm_sec << endl;
	return true;
}

BOOL DirTask(char **argv, LPSTR cur_dir)
{
	if (argv[1] != NULL) // lenh dir khong co tham so
	{
		cout << "Command \"dir\" does not support any option\n";
		return EXIT_FAILURE;
	}
	WIN32_FIND_DATA data;
	_FILETIME time_lastwrite;
	_SYSTEMTIME convert_time;
	string date;
	string time;
	char *char_date = (char *)calloc(15, sizeof(char));
	char *char_time = (char *)calloc(15, sizeof(char));
	const char add[] = "//*";
	cur_dir = strcat(cur_dir, add);
	HANDLE han = FindFirstFile(cur_dir, &data);
	cout << setw(15) << left << "DATE" << setw(15) << left << "TIME" << setw(15) << left << "TYPE" << setw(15) << left << "NAME"
		 << "\n";
	printf("=========================================================================\n");
	if (han != INVALID_HANDLE_VALUE)
	{
		do
		{
			time_lastwrite = data.ftLastWriteTime;
			FileTimeToSystemTime(&time_lastwrite, &convert_time);

			// Convert date, time to string & char *
			date = to_string(convert_time.wDay) + '/' + to_string(convert_time.wMonth) + '/' + to_string(convert_time.wYear);
			time = to_string(convert_time.wHour) + ':' + to_string(convert_time.wMinute);
			strcpy(char_date, date.c_str());
			strcpy(char_time, time.c_str());

			if (data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			{
				cout << setw(15) << left << char_date << setw(15) << left << char_time << setw(15) << left << "FILE" << setw(15) << left << data.cFileName << "\n";
			}
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				cout << setw(15) << left << char_date << setw(15) << left << char_time << setw(15) << left << "FOLDER" << setw(15) << left << data.cFileName << "\n";
			}
		} while (FindNextFileA(han, &data) != 0);
		FindClose(han);
		delete char_time;
		delete char_date;
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}

// Get Full Environment Variable
BOOL GetAllEnvironmentVar()
{
	const char *str = (char *)GetEnvironmentStrings();
	if (str == NULL)
	{
		cout << "Error get all environment variable\n";
		return EXIT_FAILURE;
	}
	int prev = 0;
	std::vector<std::string> env_strings;
	for (int i = 0;; i++)
	{
		if (str[i] == '\0')
		{
			env_strings.push_back(std::string(str + prev, str + i));
			prev = i;
			if (str[i + 1] == '\0')
			{
				break;
			}
		}
	}
	for (int i = 0; i < env_strings.size(); i++)
	{
		cout << env_strings[i] << "\n";
	}
	return EXIT_SUCCESS;
}

// Get a specific environment variable
BOOL GetSpecificEnvironmentVar(char **argv)
{
	if (argv[2] == NULL)
	{
		cout << "Required arguments\n";
		return EXIT_FAILURE;
	}
	if (argv[3] != NULL)
	{
		cout << " Command \"enva -get\"  doesn't have any arguments\n";
		return EXIT_FAILURE;
	}
	char *value = (char *)calloc(10000, sizeof(char));
	DWORD byte = 10000 * sizeof(char);
	if (GetEnvironmentVariableA(argv[2], value, byte) == 0)
	{
		cout << "No " << argv[2] << " variable was found\n";
	}
	else
	{
		cout << argv[2] << "=" << value << "\n";
	}
	delete value;
	return EXIT_SUCCESS;
}

// Set a specific environment variable
BOOL SetEnvironmentVar(char **argv)
{
	if (argv[3] == NULL)
	{
		cout << "Required arguments\n";
		return EXIT_FAILURE;
	}
	char *new_value = CombineLine(argv, 3);
	// get evironment var
	char *value = (char *)calloc(10000, sizeof(char));
	DWORD byte = 10000 * sizeof(char);
	// if variable doesn't exist, make a new variable. Else then add to existance value
	if (GetEnvironmentVariableA(argv[2], value, byte) == 0)
	{
		if (!SetEnvironmentVariableA(argv[2], new_value))
		{
			cout << "Set environment variable ERROR\n";
			return EXIT_FAILURE;
		}
	}
	else
	{
		char *p = (char*) calloc(10000,sizeof(char));
		value = strcat(p, new_value);
		if (!SetEnvironmentVariableA(argv[2], value))
		{
			cout << "Set environment variable error\n";
			return EXIT_FAILURE;
		}
	}
	// reset value

	delete value;
	return EXIT_SUCCESS;
}

BOOL AnalysisEnvironmentVar(char **argv)
{
	if (argv[1] == NULL)
	{
		printf("Required arguments.\n");
		return EXIT_FAILURE;
	}
	if (strcmp(argv[1], "-all") == 0)
	{
		if (argv[2] != NULL)
		{
			cout << "Redundant arguments\n";
			return EXIT_FAILURE;
		}
		else
		{
			GetAllEnvironmentVar();
			return EXIT_SUCCESS;
		}
	}
	else if (strcmp(argv[1], "-get") == 0)
	{
		GetSpecificEnvironmentVar(argv);
		return EXIT_SUCCESS;
	}
	else if (strcmp(argv[1], "-set") == 0)
	{
		SetEnvironmentVar(argv);
		return EXIT_SUCCESS;
	}
	else
	{
		cout << "Option " << argv[1] << " not available in command\"enva\"\n";
		return EXIT_FAILURE;
	}
}
