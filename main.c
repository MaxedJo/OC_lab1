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
    printf("%s\n", buffer);
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

int ls_long(char *str, char *argv) {
    DIR *dirp;
    struct dirent *dp;
    printf("***%s***\n", str);
    dirp = opendir(str);


    if (readdir(dirp) == NULL) {
        return -1;
    }
    chdir(str);
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] == '.') continue;
        struct stat buf;
        int st;
        st = stat(dp->d_name, &buf);
        if (st == -1)
            return -1;
        char type = '-';
        if ((buf.st_mode & S_IFDIR) == S_IFDIR) type = 'd';
        if (type == 'd' && (dp->d_name[0] != '.')) {
            char *str1 = str;
            if (str[strlen(str) - 1] != '/') str1 = concat(str1, "/");
            str1 = concat(str1, dp->d_name);
            printf("||%s %s||", str1, argv);
            printf("||%s %s||", str, argv);
            //   ls_long(str1,argv);
        }
        int j;


        // get time
        struct tm *t;
        t = localtime(&buf.st_mtime);
        printf("%c \t%ld  \t%ld \t%02dmonth%02d%02d:%02d \t%s \n", type, buf.st_nlink, buf.st_size, t->tm_mon + 1,
               t->tm_mday, t->tm_hour, t->tm_min, dp->d_name);
    }
    closedir(dirp);
    return 1;
}