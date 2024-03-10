/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** working_directory
*/

#include "ftp.h"
#include "commands.h"
#include <errno.h>
#include <stdio.h>


static char *get_absolute_path(char *relative_path)
{
    char *path;
    char *root = server_get_root(NULL);

    if (relative_path[0] == '/') {
        path = malloc(strlen(root) + strlen(relative_path) + 1);
        strcpy(path, server_get_root(NULL));
        strcat(path, relative_path);
    } else
        path = relative_path;
    return path;
}

void command_cwd(client_t *client, char **command)
{
    char *path;

    if (command[1] == NULL || strcmp(command[1], "") == 0)
        return client_send(client, "501 No directory name specified.");
    if (strncmp(command[1], "..", 2) == 0)
        return command_pwd(client, command);
    path = get_absolute_path(command[1]);
    if (chdir(path) == -1)
        return client_send(client, _550);
    getcwd(client->current_path, 1024);
    client_send(client, _250);
}

void command_pwd(client_t *client, __attribute__((unused)) char **command)
{
    char path[1024] = {0};
    char buffer[2048] = {0};

    getcwd(path, 1024);
    sprintf(buffer , "257 \"%s\" created.", path);
    client_send(client, buffer);
}

void command_cdup(client_t *client, __attribute__((unused)) char **command)
{
    if (chdir("..") == -1)
        return client_send(client, _550);
    getcwd(client->current_path, 1024);
    client_send(client, _200);
}
