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

int rmMain(int argc, char** argv)
{
    bool flag_r=false;
    vector<char *> v;
    for (int i=1;i<argc;i++) {
        if (argv[i][0] == '-')
        {
            for (unsigned j=1;j<strlen(argv[i]);j++) {
                if (argv[i][j]=='r')
                    flag_r=true;
            }
        }
        else v.push_back(argv[i]);
    }
    if (v.empty()) {
        cerr << "rm: missing operand\n";
        return 1;
    }
    for (unsigned i=0;i<v.size();i++) {
        struct stat s;
        stat(v.at(i),&s);
        if (errno) {
           perror("stat");
           errno=0;
           continue;
        }
        if (s.st_mode&S_IFDIR) {
            if (flag_r) {
                DIR *dirp=opendir(v.at(i));
                if (dirp==NULL) {
                    perror("opendir");
                    errno=0;
                    continue;
                }
                struct dirent *f;
                while ((f=readdir(dirp)))
                {
                    if (errno) {
                        perror("readdir");
                        errno=0;
                        break;
                    }
                    char recursMain[8];
                    strcpy(recursMain, "rm");
                    char recursFlags[8];
                    strcpy(recursFlags, "-r");
                    char recursFile[PATH_MAX];
                    strcpy(recursFile, f->d_name);
                    char* recursArgs[3];
                    recursArgs[0] = recursMain;
                    recursArgs[1] = recursFlags;
                    recursArgs[2] = recursFile;
                    rmMain(3,recursArgs);
                }
                closedir(dirp);
                if (errno) {
                    perror("closedir");
                    errno=0;
                    continue;
                }
            }
            else {
                cerr<<v.at(i)<< ": Is a directory but -r not set\n";
            }
        }
        if (-1==unlink(v.at(i))) {
            perror("unlink");
            errno=0;
            continue;
        }
    }
	return 0;
}

int main(int argc, char** argv)
{
	return rmMain(argc, argv);
}
