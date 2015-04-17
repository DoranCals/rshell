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
	int f = fork();
	if (f == -1) // no child was created
	{
		perror("fork");
		return 1;
	}
	else if (f == 0) // child process
	{
		char_separator<char> spaces(" \t\n\r");
		tokenizer<char_separator<char> > tokCommand(lineInput, spaces);
		if (*(tokCommand.begin() ) == "exit")
		{
			exit(0);
		}

		vector<const char*> interCommand;
		for (auto i = tokCommand.begin(); i != tokCommand.end(); i++)
		{
			// ERROR IS HERE
			const char* c = i->c_str();
			interCommand.push_back(c);
			// ERROR IS HERE
			cout << interCommand.back() << ' '; // TESTING
		}
		char** argv = new char*[interCommand.size()];
		for (size_t i = 0; i < interCommand.size(); i++)
		{
			cout << interCommand.at(i) << ' '; // TESTING
			argv[i] = (char*)interCommand.at(i);
			cout << argv[i] << endl; // TESTING
		}

		execvp(interCommand.at(0), argv);
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

int main(int argc, char **argv)
{
	char_separator<char> connectors(" \t\n\r", ";#&|");
	string lineInput;
	int prevStatus = 0;

	while (true)
	{
		char hostname[64];
		if (gethostname(hostname, 64) == 0)
			cout << getlogin() << "@" << hostname;
		cout << "$ ";

		vector<string> commands;
		getline(cin, lineInput);
		tokenizer<char_separator<char> > tokInput(lineInput, connectors);
		if (lineInput == "exit") break;
		cout << "Tokenized input:\n";

		auto i = tokInput.begin()
		while (i != tokInput.end() )
		{
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
		runProcess("ls -a -l -e -b -k /home/doran");
	}

}
