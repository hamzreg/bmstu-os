#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#include "constants.h"
#include "stat_content.h"

void print_cmdline_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/cmdline", pid);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len = fread(buf, 1, BUF_SIZE, file);
    buf[len] = '\0';

    fprintf(result, "CMDLINE\n%s\n\n", buf);
    fclose(file);
}

void print_cwd_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/cwd", pid);

    char buf[BUF_SIZE];
    int len = readlink(path, buf, BUF_SIZE);
    buf[len] = '\0';

    fprintf(result, "CWD\n%s\n\n", buf);
}

void print_environ_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/environ", pid);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len;

    fprintf(result, "ENVIRON\n");

    while ((len = fread(buf, 1, BUF_SIZE, file)) > 0)
    {
        for (int i = 0; i < len; i++)
        {
            if (buf[i] == 0)
            {
                buf[i] = '\n';
            }
        }

        buf[len] = '\0';
        fprintf(result, "%s", buf);
    }

    fclose(file);
    fprintf(result, "\n");
}

void print_exe_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/exe", pid);

    char buf[BUF_SIZE];
    int len = readlink(path, buf, BUF_SIZE);
    buf[len] = '\0';

    fprintf(result, "EXE\n%s\n\n", buf);
}

void print_fd_info(const char *pid, FILE *result)
{
    char fd_path[PATH_MAX];
    snprintf(fd_path, PATH_MAX, "/proc/%s/fd", pid);

    DIR *dp = opendir(fd_path);
    struct dirent *dirp;
    char path[BUF_SIZE];
    char buf[BUF_SIZE];

    fprintf(result, "FD\n");

    while ((dirp = readdir(dp)) != NULL)
    {
        if ((strcmp(dirp->d_name, ".") != 0) &&
            (strcmp(dirp->d_name, "..") != 0))
        {
            sprintf(path, "%s%s", fd_path, dirp->d_name);
            int len = readlink(path, buf, BUF_SIZE);
            buf[len] = '\0';

            fprintf(result, "%s -> %s\n", dirp->d_name, buf);
        }
    }

    closedir(dp);
    fprintf(result, "\n");
}

void print_root_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/root", pid);

    char buf[BUF_SIZE];
    int len = readlink(path, buf, BUF_SIZE);
    buf[len] = '\0';

    fprintf(result, "ROOT\n%s\n\n", buf);
}

void print_stat_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/stat", pid);

    char buf[BUF_SIZE];
    FILE *file = fopen(path, "r");
    fread(buf, 1, BUF_SIZE, file);
    char *elem = strtok(buf, " ");
    int i = 0;

    fprintf(result, "STAT\n");

    while (elem != NULL)
    {
        fprintf(result, content[i], elem);
        elem = strtok(NULL, " ");
        i++;
    }

    fclose(file);
    fprintf(result, "\n");
}

void print_io_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/io", pid);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len;

    fprintf(result, "IO\n");

    while ((len = fread(buf, 1, BUF_SIZE, file)) > 0)
    {
        for (int i = 0; i < len; i++)
        {
            if (buf[i] == 0)
            {
                buf[i] = '\n';
            }
        }

        buf[len] = '\0';
        fprintf(result, "%s", buf);
    }

    fclose(file);
    fprintf(result, "\n");
}

void print_maps_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/maps", pid);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len;

    fprintf(result, "MAPS\n");

    while ((len = fread(buf, 1, BUF_SIZE, file)) > 0)
    {
        buf[len] = '\0';
        fprintf(result, "%s", buf);
    }

    fclose(file);
    fprintf(result, "\n");
}

void print_comm_info(const char *pid, FILE *result)
{
    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/comm", pid);
    FILE *file = fopen(path, "r");

    char buf[BUF_SIZE];
    int len = fread(buf, 1, BUF_SIZE, file);
    buf[len] = '\0';

    fprintf(result, "COMM\n%s\n", buf);
    fclose(file);
}

void print_task_info(const char *pid, FILE *result)
{
    char fd_path[PATH_MAX];
    snprintf(fd_path, PATH_MAX, "/proc/%s/task", pid);

    DIR *dp = opendir(fd_path);
    struct dirent *dirp;

    fprintf(result, "TASK\n");

    while ((dirp = readdir(dp)) != NULL)
    {
        if ((strcmp(dirp->d_name, ".") != 0) &&
            (strcmp(dirp->d_name, "..") != 0))
        {
            fprintf(result, "%s\n", dirp->d_name);
        }
    }

    closedir(dp);
}

void get_pid(int argc, char *argv[], char *pid)
{
    if (argc < 2)
    {
        snprintf(pid, PID_LEN, "self");
    }
    else
    {
        snprintf(pid, PID_LEN, "%s", argv[1]);
    }
}

int main (int argc, char *argv[])
{
    char pid[PID_LEN];
    get_pid(argc, argv, pid);

    FILE *result = fopen("info.txt", "w");

    print_cmdline_info(pid, result);
    print_cwd_info(pid, result);
    print_exe_info(pid, result);
    print_fd_info(pid, result);
    print_root_info(pid, result);
    print_stat_info(pid, result);
    print_environ_info(pid, result);
    print_io_info(pid, result);
    print_maps_info(pid, result);
    print_comm_info(pid, result);
    print_task_info(pid, result);

    fclose(result);

    return 0;
}
