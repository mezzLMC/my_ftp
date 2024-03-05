/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** working_directory
*/

#include "ftp.h"

void command_cwd(client_t *client, char **command)
{
    if (command[1] == NULL || strcmp(command[1], "") == 0)
        return client_send(client, "501 No directory name specified.");
    if (chdir(command[1]) == -1)
        return client_send(client, "550 Failed to change directory.");
    getcwd(client->current_path, 1024);
    client_send(client, _250);
}

void command_pwd(client_t *client, __attribute__((unused)) char **command)
{
    char path[1024] = {0};
    char buffer[2048] = {0};
    char *root = server_get_root(NULL);

    getcwd(path, 1024);
    if (strcmp(path, root) == 0)
        sprintf(buffer, _257("/"));
    else
        sprintf(buffer, "257 \"%s\" created.", path + strlen(root));
    client_send(client, buffer);
}

void command_cdup(client_t *client, __attribute__((unused)) char **command)
{
    chdir("..");
    getcwd(client->current_path, 1024);
    client_send(client, _200);
}
