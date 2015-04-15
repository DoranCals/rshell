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

typedef tokenizer<char_separator<char> > tokenizedInput;

void runProcess(int argc, char **argv)
{
}

int main(int argc, char **argv)
{
	char_separator<char> delim(";#&| \t\n\r", ";#&|");
	string lineInput;

	while (true)
	{
		cout << "$ ";
		getline(cin, lineInput);
		//wedgeConnectors(lineInput);
		tokenizer<char_separator<char> > tokenizedInput(lineInput, delim);
		if (lineInput == "exit") break;
		cout << "Tokenized input:\n";
		for (auto i = tokenizedInput.begin(); i != tokenizedInput.end(); i++)
		{
			cout << *i << endl;
		}
		cout << "Original input: " << lineInput << endl;
	}

}
