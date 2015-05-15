#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;
using namespace boost;

//typedef tokenizer<char_separator<char> > tokInput;

char RSF_VERBOSE = 1;

// Runs a process with arguments.
// The process name is specified by the first space-separated word.
// Input and output redirection are handled here.
// Returns the process's exit status, or 1 if the process didn't start.
int runPipe(string lineInput)
{
	char_separator<char> redirectors(" \t\n\r", "<>");
	tokenizer<char_separator<char> > tokCommand(lineInput, redirectors);

	if (tokCommand.begin() == tokCommand.end() ) return 0;

	if (*(tokCommand.begin() ) == "exit") exit(0);
	int f = fork();
	if (f == -1)
	{
		perror("fork");
		return 1;
	}
	else if (f == 0)
	{
		int argc = 1; // leave space for the null pointer
		for (auto i = tokCommand.begin(); (i != tokCommand.end() and true); i++)
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
	else
	{
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

int main(int argc, char** argv)
{
	char_separator<char> connectors(" \t\n\r", ";#&|");
	string lineInput;
	int prevStatus = 0;
	char flags = 0;

	if (argc > 1)
	{
		// Look for flags
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
			{
				for (size_t j = 1; j < strlen(argv[i]); j++)
				{
					if (argv[i][j] == 'v') flags = flags | RSF_VERBOSE;
				}
			}
		}
	}

	while (true)
	{
		// Prompt; displays username and hostname
		char* username;
		char hostname[64];
		username = getlogin();
		if (username == NULL) perror("getlogin");
		if (gethostname(hostname, 64) ) perror("gethostname");
		if (username)
		{
			cout << username;
			if (!errno) cout << "@" << hostname;
		}
		cout << "$ ";

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
				prevStatus = runPipe(thisCommand);
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
