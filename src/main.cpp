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

typedef tokenizer<char_separator<char> > tokInput;

// Runs a process with arguments.
// The process name is specified by the first space-separated word in the string.
// Returns zero on success, non-zero on failure
int runProcess(string lineInput)
{
	char_separator<char> spaces(" \t\n\r");
	tokenizer<char_separator<char> > tokCommand(lineInput, spaces);
	if (*(tokCommand.begin() ) == "exit") exit(0); // quit immediately if "exit" is invoked

	int argc = 0; // How many arguments are there? Tokenizer will find out
	for (auto i = tokCommand.begin(); i != tokCommand.end(); i++)
	{
		argc++;
	}
	char** argv = new char*[argc+1];
	{
		int j = 0;
		for (auto i = tokCommand.begin(); i != tokCommand.end(); i++)
		{
			argv[j] = new char[i->size()+1];
			argv[j] = (char*)i->c_str();
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
		cout << "Executing..." << endl;
		execvp(argv[0], argv);
		cout << "Couldn't execute." << endl;
		// if execvp() didn't work, deallocate the c-string arguments passed into it
		for (int i = 0; i < argc+1; i++)
		{
			delete argv[i];
		}
		delete argv;
		perror("execvp");
		return 1;
	}
	else // parent process
	{
		int status = 1;
		int p = wait(&status);
		if (p == -1)
		{
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

		vector<string> commands;

		getline(cin, lineInput);

		tokenizer<char_separator<char> > tokInput(lineInput, connectors);

		auto i = tokInput.begin();
		while (i != tokInput.end() )
		{
			// Will search for connectors and reformat the commands based on those
			auto j = i;

			/*
			if ( (*i == "&" and j != tokInput.end() and *j == "&")
				or (*i == "|" and j != tokInput.end() and *j == "|")
				or *i == ";" or *i == "#" )
			{
				
			}
			else
			{

			}
			*/
			i++;
		}
		// This should go in the while loop above, so it'll run multiple times with connectors
		prevStatus = runProcess(lineInput); 

		cout << prevStatus << endl; // TESTING
	}

}
