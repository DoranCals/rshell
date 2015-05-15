#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
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

int mvMain(int argc, char** argv)
{
    if (argc!=3) {
        cerr<<"mv: missing file operand\n";
        return 1;
    }
    struct stat s2;
    stat(argv[2],&s2);
    if (errno) {
        errno = 0;
        link(argv[1],argv[2]);
        if (errno) {
            perror("link");
            return 1;
        }
        unlink(argv[1]);
        if (errno) {
            perror("unlink");
            return 1;
        }
    }
    else
    {
        if (s2.st_mode&S_IFDIR) {
            char c[PATH_MAX];
            strcpy(c, argv[2]);
            strcat(c, "/");
            strcat(c, argv[1]);
            link(argv[1], c);
            if (errno) {
                perror("link");
                return 1;
            }
            unlink(argv[1]);
            if (errno) {
                perror("unlink");
                return 1;
            }
        }
        else {
            cerr<<"mv: cannot move file into a file\n";
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
	return mvMain(argc, argv);
}
