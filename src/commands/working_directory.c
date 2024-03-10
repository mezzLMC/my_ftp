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

void command_cwd(client_t *client, char **command)
{
    char *path;

    if (command[1] == NULL || strcmp(command[1], "") == 0)
        return client_send(client, "501 No directory name specified.");
    path = command[1];
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
    sprintf(buffer, "257 \"%s\" created.", path);
    client_send(client, buffer);
}

void command_cdup(client_t *client, __attribute__((unused)) char **command)
{
    if (chdir("..") == -1)
        return client_send(client, _550);
    getcwd(client->current_path, 1024);
    client_send(client, _200);
}
