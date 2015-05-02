#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <libgen.h>
#include <linux/limits.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

char LSF_LSTALL = 1; // include dotfiles
char LSF_DETAIL = 4; // more information about each file
char LSF_RECURS = 8; // recurse through subdirs


// Compares two char arrays, disregarding case.
// For use with std::sort()
bool sortCharArray(const char* c, const char* d)
{
	char* a = new char[strlen(c) + 1];
	char* b = new char[strlen(d) + 1];
	bool result;

	strcpy(a, c);
	strcpy(b, d);
	for (unsigned i = 0; i < strlen(a); i++)
		a[i] = tolower(a[i]);
	for (unsigned i = 0; i < strlen(b); i++)
		b[i] = tolower(b[i]);

	result = strcmp(a, b) < 0;
	delete[] a;
	delete[] b;
	return result;
}

// Prints the given filenames to stdout.
void listFiles(vector<char*> v, int w)
{
	int currentWidth = 0;
	for (unsigned i = 0; i < v.size(); i++)
	{
		currentWidth += strlen(basename(v.at(i) ) );
		if (currentWidth > w)
		{
			cout << endl;
			currentWidth = strlen(basename(v.at(i) ) );
		}
		printf("%s  ", basename(v.at(i) ) );
	}
	cout << endl;
	return;
}


// Prints the given filenames and metadata to stdout.
void listFilesMetadata(vector<char*> v);
#include "listFilesMetadata.h"

int main(int argc, char** argv)
{
	char flags = 0; // argument flags
	vector<char*> fileArgs; // arguments that aren't flags
	vector<char*> filesInDir; // filenames to look at later

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-') // it's a flag, not a file
		{
			for (unsigned j = 1; j < strlen(argv[i]); j++)
			{
				if (argv[i][j] == 'a')
					flags = flags|LSF_LSTALL;
				if (argv[i][j] == 'l')
					flags = flags|LSF_DETAIL;
				if (argv[i][j] == 'R')
					flags = flags|LSF_RECURS;
			}
		}
		else // it's a file
			fileArgs.push_back(argv[i]);
	}
	if (fileArgs.empty() )
		fileArgs.push_back( (char*)"."); // by default, list the working directory

	DIR* thisDir = 0; // look at the current directory
	for (unsigned i = 0; i < fileArgs.size(); i++)
	{
		thisDir = opendir(fileArgs.at(i) );
		if (!thisDir)
		{
			perror("opendir");
			return errno;
		}
		dirent* thisFile; // look at each file in the directory
		while ( (thisFile = readdir(thisDir) ) )
		{
			if (errno)
			{
				perror("readdir");
				return errno;
			}

			// If file is a dotfile AND -a wasn't given, skip it
			if ( (flags & LSF_LSTALL) or (thisFile->d_name[0] != '.') )
			{
				char* fileName = new char[PATH_MAX];
				strcpy(fileName, fileArgs.at(i) );
				strcat(fileName, "/");
				strcat(fileName, thisFile->d_name);
				filesInDir.push_back(fileName);	
			}
		}
		
		// Time to actually display the information
		sort(filesInDir.begin(), filesInDir.end(), sortCharArray);
		if ( (fileArgs.size() > 1) or (flags & LSF_RECURS) )
			printf("%s:\n", fileArgs.at(i));

		if (flags & LSF_DETAIL)
			listFilesMetadata(filesInDir);
		else
			listFiles(filesInDir, 80);

		if ( (fileArgs.size() > 1) and (i < fileArgs.size() - 1) )
			cout << endl;

		// Cleanup
		for (unsigned i = 0; i < filesInDir.size(); i++)
			delete[] filesInDir.at(i);
		filesInDir.clear();

		closedir(thisDir);
		if (errno)
		{
			perror("closedir");
			return errno;
		}
	}

	return 0;
}
