#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>

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

void print_page(uint64_t address, uint64_t data, FILE *out)
{
    fprintf(out, "0x%-16lx : %-16lx %-10ld %-10ld %-10ld %-10ld\n",
        address,
        data & (((uint64_t)1 << 55) - 1),
        (data >> 55) & 1,
        (data >> 61) & 1,
        (data >> 62) & 1,
        (data >> 63) & 1);
}

void print_pagemap_info(const char *pid, FILE *result)
{
    fprintf(result, "PAGEMAP\n");
    fprintf(result, "       addr        : pfn           soft-dirty file/shared swapped present\n");

    char path[PATH_MAX];
    snprintf(path, PATH_MAX, "/proc/%s/maps", pid);
    FILE *maps_file = fopen(path, "r");

    snprintf(path, PATH_MAX, "/proc/%s/pagemap", pid);
    int pm_fd = open(path, O_RDONLY);

    char buf[BUF_SIZE] = "\0";
    int len;

    while ((len = fread(buf, 1, BUF_SIZE, maps_file)) > 0)
    {
        for (int i = 0; i < len; i++)
        {
            if (buf[i] == 0)
            {
                buf[i] = '\n';
            }
        }

        buf[len] = '\0';

        char *save_row;
        char *row = strtok_r(buf, "\n", &save_row);

        while (row)
        {
            char *addresses = strtok(row, " ");

            char *start_str, *end_str;

            if ((start_str = strtok(addresses, "-")) &&
                (end_str = strtok(NULL, "-")))
            {

                uint64_t start = strtoul(start_str, NULL, 16);
                uint64_t end = strtoul(end_str, NULL, 16);

                for(uint64_t i = start; i < end; i += sysconf(_SC_PAGE_SIZE))
                {
                    uint64_t offset;
                    uint64_t index = ((i / sysconf(_SC_PAGE_SIZE))
                                         * sizeof(offset));

                    pread(pm_fd, &offset, sizeof(offset), index);
                    print_page(i, offset, result);
                }
            }

            row = strtok_r(NULL, "\n", &save_row);
        }

    }

    fclose(maps_file);
    close(pm_fd);
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
    print_pagemap_info(pid, result);

    fclose(result);

    return 0;
}
