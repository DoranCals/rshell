#ifndef LS_METADATA_H
#define LS_METADATA_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <pwd.h>
#include <math.h>
#include <libgen.h>

#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

// Prints the given filenames and metadata to stdout.
void listFilesMetadata(vector<char*> v)
{
	struct stat fileStat; // leave space to work with a single file
	int blocks = 0; // total blocks used by this directory
	int fileSizeColumn = 0; // set filesize column width

	// Determine how to format the output first
	for (unsigned i = 0; i < v.size(); i++)
	{
		stat(v.at(i), &fileStat);
		if (errno)
		{
			perror("stat");
			exit(errno);
		}
		blocks += fileStat.st_blocks;
		// how many digits is the largest file's size?
		if ( (log10(fileStat.st_size) + 1) > fileSizeColumn)
			fileSizeColumn = log10(fileStat.st_size) + 1;
	}
	printf("total %i\n", blocks/2);

	for (unsigned i = 0; i < v.size(); i++)
	{
		stat(v.at(i), &fileStat);
		if (errno)
		{
			perror("stat");
			exit(errno);
		}

		// File characteristics
		if (fileStat.st_mode & S_IFREG)
			cout << '-';
		else if (fileStat.st_mode & S_IFDIR)
			cout << 'd';
		else if (fileStat.st_mode & S_IFBLK)
			cout << 'b';
		else if (fileStat.st_mode & S_IFLNK)
			cout << 'l';
		else if (fileStat.st_mode & S_IFCHR)
			cout << 'c';
		else if (fileStat.st_mode & S_IFSOCK)
			cout << 's';
		else if (fileStat.st_mode & S_IFIFO)
			cout << 'f';
		else
			cout << '?';

		// Permissions
		cout
		<< ( (fileStat.st_mode & S_IRUSR) ? 'r' : '-')
		<< ( (fileStat.st_mode & S_IWUSR) ? 'w' : '-')
		<< ( (fileStat.st_mode & S_IXUSR) ? 'x' : '-')
		<< ( (fileStat.st_mode & S_IRGRP) ? 'r' : '-')
		<< ( (fileStat.st_mode & S_IWGRP) ? 'w' : '-')
		<< ( (fileStat.st_mode & S_IXGRP) ? 'x' : '-')
		<< ( (fileStat.st_mode & S_IROTH) ? 'r' : '-')
		<< ( (fileStat.st_mode & S_IWOTH) ? 'w' : '-')
		<< ( (fileStat.st_mode & S_IXOTH) ? 'x' : '-')
		<< ' ';

		// Hard links
		printf("%li ", fileStat.st_nlink);

		// Owner and group
		passwd* fileOwner;
		fileOwner = getpwuid(fileStat.st_uid);
		if (errno)
		{
			perror("getpwuid");
			exit(errno);
		}
		printf("%s %s ", fileOwner->pw_name, fileOwner->pw_name);

		// Filesize
		printf("%*li ", fileSizeColumn, fileStat.st_size);

		// Timestamp
		tm timeStamp;
		localtime_r(&fileStat.st_mtime, &timeStamp);
		char timeStampString[20];
		strftime( (char*)timeStampString, 20, "%b %d %H:%M", &timeStamp);
		printf("%s ", timeStampString);

		// Filename (finally!)
		printf("%s ", basename(v.at(i) ) );
		cout << endl;

	}
	return;
}

#endif
