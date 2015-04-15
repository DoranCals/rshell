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
int runProcess(string s, char **argv)
{
	int f = fork();
	if (f == -1)
	{
		perror("fork");
		return 1;
	}
	else if (f == 0)
	{
		//execvp(s, argv)
		perror("execvp");
		return 0;
	}
	else
	{
		int status = 1;
		int p = wait(&status);
		if (p == -1)
		{
			return 0;
		}
		return status;
	}
}

int main(int argc, char **argv)
{
	char_separator<char> delim(" \t\n\r", ";#&|");
	string lineInput;

	while (true)
	{
		char hostname[64];
		if (gethostname(hostname, 64) == 0)
			cout << getlogin() << "@" << hostname;
		cout << "$ ";

		vector<string> commands;
		getline(cin, lineInput);
		tokenizer<char_separator<char> > tokInput(lineInput, delim);
		if (lineInput == "exit") break;
		cout << "Tokenized input:\n";
		for (auto i = tokInput.begin(); i != tokInput.end(); i++)
		{
			cout << *i << endl;

		}
	}

}
