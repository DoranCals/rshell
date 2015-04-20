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

// Runs a process with arguments.
// The process name is specified by the first space-separated word.
// Returns the process's exit status, or 1 if the process didn't start.
int runProcess(string lineInput)
{
	char_separator<char> spaces(" \t\n\r");
	tokenizer<char_separator<char> > tokCommand(lineInput, spaces);

	if (tokCommand.begin() == tokCommand.end() ) return 0; // do nothing

	if (*(tokCommand.begin() ) == "exit") exit(0); // quit rshell immediately

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
		execvp(argv[0], argv);
		// if execvp() didn't work, deallocate the c-strings passed into it
		for (int i = 0; i < argc; i++)
		{
			//cout << "Deleting argv[" << i << "]... " << flush; // TESTING
			delete [] argv[i];
			//cout << "done." << endl; // TESTING
		}
		//cout << "Deleting argv... " << endl; // TESTING
		delete [] argv;
		//cout << "done."; // TESTING
		perror("execvp");
		return 1;
	}
	else // parent process
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

	while (true)
	{
		// Prompt; displays username and hostname
		char hostname[64];
		if (gethostname(hostname, 64) == 0)
			cout << getlogin() << "@" << hostname;
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
