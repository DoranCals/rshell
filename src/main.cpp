#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/limits.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>


using namespace std;
using namespace boost;

//typedef tokenizer<char_separator<char> > tokInput;

struct sigaction sigInterruptOld;
struct sigaction sigInterrupt;

int changeDir(const char* n)
{
	// Change the current working directory to newPath.
	// If newPath is "-", change to the previous directory.
	// If newPath is empty, change to the user's home directory.
	// Return zero on success, non-zero on failure
	
	char newPath[PATH_MAX];
	if (strlen(n) == 0)
	{
		// newPath becomes the user's home directory
		char* homePath = getenv("HOME");
		if (!homePath)
		{
			perror("getenv $HOME");
			return 3;
		}
		strcpy(newPath, homePath);
	}
	else if (strlen(n) == 1 and n[0] == '-')
	{
		// newPath becomes the previous directory
		char* oldPath = getenv("OLDPWD");
		if (!oldPath)
		{
			perror("getenv $OLDPWD");
			return 3;
		}
		strcpy(newPath, oldPath);
	}
	else
	{
		// newPath becomes the first argument
		strcpy(newPath, n);
	}
	
	char currentPath[PATH_MAX];

	// Store the old path first
	char* oldPath = getenv("PWD");
	if (!oldPath)
	{
		cerr << "// Store the old path first\n";
		perror("getenv $PWD");
		return 3;
	}

	if (chdir(newPath) == -1)
	{
		perror("chdir");
		return 1;
	}

	// Record the old path AFTER changing dir,
	// in case chdir fails
	setenv("OLDPWD", oldPath, 1);
	if (errno)
	{
		cerr << "// Record the old path AFTER changing dir\n";
		perror("setenv $OLDPWD");
		return 2;
	}
	// Now record the new path
	getcwd(currentPath, PATH_MAX);
	if (errno)
	{
		cerr << "// Now record the new path\n";
		perror("getcwd");
		return 4;
	}
	setenv("PWD", currentPath, 1);
	if (errno)
	{
		perror("setenv $PWD");
		return 2;
	}

	return 0;
}

int runProcess(string lineInput)
{
	// Run either an internal shell command or an external process.
	// Arguments should be space-separated.
	// Returns the process's exit status, or 1 if the process didn't start.

	char_separator<char> spaces(" \t\n\r");
	tokenizer<char_separator<char> > tokCommand(lineInput, spaces);

	if (tokCommand.begin() == tokCommand.end() ) return 0; // do nothing

	if (*(tokCommand.begin() ) == "exit") exit(0); // quit rshell immediately
	
	if (*(tokCommand.begin() ) == "cd")
	{
		auto a = tokCommand.begin();
		a++;
		char newPath[PATH_MAX];
		if (a == tokCommand.end() )
		{
			newPath[0] = 0;
		}
		else
		{
			for (unsigned i = 0; i < a->size(); i++)
			{
				newPath[i] = a->c_str()[i];
			}
			newPath[a->size()] = 0;
		}
		return changeDir(newPath); // change working directory
	}

	int argc = 1; // leave space for the null pointer
	for (auto i = tokCommand.begin(); i != tokCommand.end(); i++)
	{
		argc++;
	}

	char** argv = new char*[argc];
	{
		int j = 0;
		for (auto i = tokCommand.begin(); i != tokCommand.end(); i++)
		{
			argv[j] = new char[i->size()+1];
			for (unsigned k = 0; k < i->size(); k++)
			{
				argv[j][k] = i->c_str()[k];
			}
			argv[j][i->size()] = 0;
			j++;
		}
		argv[j] = 0;
	}

	int f = fork();
	if (f == -1) // no child was created
	{
		perror("fork");
		return 1;
	}
	else if (f == 0) // child process
	{
		sigaction(SIGINT, &sigInterruptOld, &sigInterrupt);
		execvp(argv[0], argv);
		// if execvp() didn't work, deallocate the c-strings passed into it
		for (int i = 0; i < argc; i++)
		{
			delete [] argv[i];
		}
		delete [] argv;
		perror("execvp");
		return 1;
	}
	else // parent process
	{
		// deallocate c-strings immediately, as they're not needed here
		for (int i = 0; i < argc; i++)
		{
			delete [] argv[i];
		}
		delete [] argv;

		int status = 1;
		int p = wait(&status);
		if (p == -1)
		{
			perror("wait");
			return 1;
		}
		return status;
	}
}

void displayPrompt(ostream &out)
{		
	// Print the command prompt

	char* username = getenv("LOGNAME");
	if (!username)
		perror("getenv $LOGNAME");
	else
		out << username;

	char hostname[64];
	if (gethostname(hostname, 64) )
		perror("hostname");
	else
		out << '@' << hostname;

	out << flush;

	// display working directory
	char* workDir = getenv("PWD");
	if (!workDir)
		perror("getenv $PWD");
	else
	{
		out << ": ";
		char* homePath = getenv("HOME");
		if (!homePath)
		{
			perror("getenv $HOME");
		}
		else if (strncmp(homePath, workDir, strlen(homePath) ) == 0)
		{
			workDir += strlen(homePath);
			out << '~';
		}
		out << workDir;
	}
	
	
	out << " $ " << flush;

	return;
}

int main(int argc, char** argv)
{
	sigInterrupt.sa_handler = SIG_IGN;
	sigInterruptOld.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sigInterrupt, &sigInterruptOld);

	char_separator<char> connectors(" \t\n\r", ";#&|");
	string lineInput;
	int prevStatus = 0;

	while (true)
	{
		displayPrompt(cout);

		string thisCommand = "";

		if (cin.eof() )
		{
			cout << "^D" << endl;
			exit(0);
		}
		getline(cin, lineInput);
		lineInput += " ;";

		tokenizer<char_separator<char> > tokInput(lineInput, connectors);

		auto i = tokInput.begin();
		while (i != tokInput.end() )
		{
			// If a connector is found, execute the stored command.
			if (*i == "&" or *i == "|" or *i == ";" or *i == "#" )
			{
				prevStatus = runProcess(thisCommand);
				thisCommand = "";
				if (*i == "&" or *i == "|")
				{
					auto j = i; j++;
					if (j == tokInput.end() ) break;
					if (*j == *i)
					{
						if (*i == "|")
						{
							i++;
							if (prevStatus == 0) break;
						}
						if (*i == "&")
						{
							i++;
							if (prevStatus != 0) break;
						}
					}
				}
				else if (*i == "#") break; // ignore the rest of the line
				// Semicolons will be handled automatically;
				// the token iterator simply moves on to the next command.
			}
			else // Not a connector, so treat it as a command or argument.
			{
				thisCommand += *i;
				thisCommand += " ";
			}
			i++;
		}
	}
}
