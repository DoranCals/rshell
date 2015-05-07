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

void handle_flags(const int &argc, char **argv, bool &flag_r) {
    for (int i=0;i<argc;i++)
        if (strcmp(argv[i],"-r")==0)
            flag_r=true;
}

void rm_dirs(const char *dir_name) {
    DIR *dirp;
    if (NULL==(dirp=opendir(dir_name))) {
        perror("opendir");
        return;
    }
    struct dirent *filespecs;
    errno=0;
    while (NULL!=(filespecs=readdir(dirp))) {
        if (strcmp(filespecs->d_name,".")==0||strcmp(filespecs->d_name,"..")==0)
            continue;
        string file_name=dir_name;
        file_name+="/";
        file_name.append(filespecs->d_name);
        struct stat s;
        if (-1==stat(file_name.c_str(),&s)) {
            perror("stat");
            continue;
        }
        if (s.st_mode&S_IFDIR)
            rm_dirs(file_name.c_str());
        else if (-1==unlink(file_name.c_str())) {
            perror("unlink");
            continue;
        }
    }
    if (errno) {
        perror("readdir");
        return;
    }
    if (-1==rmdir(dir_name)) {
        perror("rmdir");
        return;
    }
}

int rmMain(int argc, char** argv)
{
    if (argc<2) {
        cerr<<"rm: missing operand\n";
        return 1;
    }
    bool flag_r=false;
    handle_flags(argc,argv,flag_r);
    for (int i=1;i<argc;i++) {
        if (*argv[i]=='-')
            continue;
        struct stat s;
        if (-1==stat(argv[i],&s)) {
            perror("stat");
            continue;
        }
        if (s.st_mode&S_IFDIR) {
            if (!flag_r) {
                cerr<<"rm: cannot remove `"<<argv[i]<<"': Is a directory\n";
                continue;
            }
            rm_dirs(argv[i]);
        }
        else if (-1==unlink(argv[i])) {
            perror("unlink");
            continue;
        }
    }
	return 0;
}

int main(int argc, char** argv)
{
	return rmMain(argc, argv);
}
