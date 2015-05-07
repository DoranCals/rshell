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
    if (-1==stat(argv[2],&s2)) {
        errno=0;
        if (-1==link(argv[1],argv[2])) {
            perror("link");
            return 1;
        }
        if (-1==unlink(argv[1])) {
            perror("unlink");
            return 1;
        }
    }
    else {
        if (s2.st_mode&S_IFDIR) {
            string file_name=argv[2];
            file_name+="/";
            file_name.append(argv[1]);
            if (-1==link(argv[1],file_name.c_str())) {
                perror("link");
                return 1;
            }
            if (-1==(unlink(argv[1]))) {
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
