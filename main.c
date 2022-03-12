#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>

const int STATUS_OK = 1;
const int STATUS_ERROR_OPEN_DIR = -1;
const int STATUS_ERROR_INPUT_PARAM = 0;

int readDir(char *dirName);

int ls(char *str);

int main(int argc, char *argv[]) {
    int status;
    status = readDir(argv[1]);
    if (status == STATUS_ERROR_OPEN_DIR)
        printf("Error: file can't open\n");
    if (status == STATUS_ERROR_INPUT_PARAM)
        printf("Error: input parameter error\n");
    return 0;
}

char buffer[PATH_MAX];

int readDir(char *dirName) {
    DIR *pDir;
    struct dirent *dp;
    struct stat fileStat;
    if (lstat(dirName, &fileStat) == -1) {
        perror(dirName);
        return STATUS_ERROR_OPEN_DIR;
    }
    if (!S_ISDIR(fileStat.st_mode)) {
        return STATUS_OK;
    }

    if ((pDir = opendir(dirName)) == NULL) {
        perror(dirName);
        return STATUS_ERROR_OPEN_DIR;
    }
    size_t len = strlen(dirName);
    strncpy(buffer, dirName, len);
    printf("%s - dir\n", buffer);
    ls(buffer);
    while ((dp = readdir(pDir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        if (buffer[len - 1] != '/')
            buffer[len] = '/';
        buffer[len + 1] = '\0';
        strcat(buffer, dp->d_name);
        if (readDir(buffer) != STATUS_OK) {
            return STATUS_ERROR_OPEN_DIR;
        }
    }
    return STATUS_OK;
}

int ls(char *str) {
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(str);
    while ((dp = readdir(dirp)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }
        //----------------------------------------------

        //
        //  char* full;
        //  strcat(full,str);
        //  strcat(full,"/");
        //  strcat(full,dp->d_name);

        // Например этот блок раскоменти
        //struct stat statbuf;
        //mode_t modes;
        //stat(dp->d_name,&statbuf);
        //modes = statbuf.st_mode;
        //if (!S_ISDIR(modes) && (modes & S_IRWXU) == S_IXUSR){
        //  printf("%s--",dp->d_name);
        // }


        //-----------------------------------------------------------------------
        // Или вот этот способ
        //    struct stat fileStat;
        //   lstat(dp->d_name, &fileStat);
        //   if (!S_ISDIR(fileStat.st_mode)) {
        //       printf("%s---",dp->d_name);
        // }
        //-------------------------------------------------------------------
        printf("%s/%s\n", str, dp->d_name);
    }
    closedir(dirp);
    return 1;
}