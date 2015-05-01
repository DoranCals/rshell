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

// Compares two C-style strings without case-sensitivity
// For use with std::sort()
bool sortCharArrays(const char* charArray1, const char* charArray2)
{
	char* a = new char[strlen(charArray1) + 1];
	char* b = new char[strlen(charArray2) + 1];
	bool result;

	strcpy(a, charArray1);
	strcpy(b, charArray2);

	for (unsigned i = 0; i < strlen(a); i++)
		a[i] = tolower(a[i]);
	for (unsigned i = 0; i < strlen(b); i++)
		b[i] = tolower(b[i]);

	result = (strcmp(a, b) < 0);
	delete[] a;
	delete[] b;
	return result;
}


// Prints the given filenames to stdout.
void listFiles(vector<char*> v)
{
	for (unsigned i = 0; i < v.size(); i++)
		printf("%s  ", basename(v.at(i) ) );
	cout << endl;
	return;
}


// Prints the given filenames and metadata to stdout.
void listFilesMetadata(vector<char*> v)
{

}


int main(int argc, char** argv)
{
	char flags = 0;
	char fileName[PATH_MAX]; // Don't allocate new memory for filenames
	vector<char*> fileArgs;
	vector<char*> filesInDir;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-') // it's a flag, not a file argument
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
		else
			fileArgs.push_back(argv[i]);
	}
	if (fileArgs.empty() )
		fileArgs.push_back( (char*)"."); // by default, list the working directory

	DIR* thisDir = 0;
	for (unsigned i = 0; i < fileArgs.size(); i++)
	{
		thisDir = opendir(fileArgs.at(i) );
		if (!thisDir)
		{
			perror("opendir");
			return errno;
		}
		dirent* thisFile = readdir(thisDir);
		if (errno)
		{
			perror("readdir");
			return errno;
		}
		while (thisFile)
		{
			if ( (flags & LSF_LSTALL) or (thisFile->d_name[0] != '.') )
			{
				//strcpy(fileName, fileArgs.at(i) );
				//strcat(fileName, "/");
				strcat(fileName, thisFile->d_name);
				filesInDir.push_back(fileName);	
			}
			thisFile = readdir(thisDir);
			if (errno)
			{
				perror("readdir");
				return errno;
			}
		}
		
		// Alphabetical order is the reasonable thing to do
		sort(filesInDir.begin(), filesInDir.end(), sortCharArrays);

		// Time to actually display the information
		if ( (fileArgs.size() > 1) or (flags & LSF_RECURS) )
			printf("%s:\n", fileArgs.at(i));

		if (flags & LSF_DETAIL)
			listFilesMetadata(filesInDir);
		else
			listFiles(filesInDir);

		if ( (fileArgs.size() > 1) and (i < fileArgs.size() - 1) )
			cout << endl;

		closedir(thisDir);
		if (errno)
		{
			perror("closedir");
			return errno;
		}
	}

	return 0;
}
