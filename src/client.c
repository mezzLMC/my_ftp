/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** client
*/

#include "ftp.h"
#include "commands.h"

command_func_t client_find(char **buffer_tab)
{
    int i = 0;
    char *command = buffer_tab[0];

    for (i = 0; command_funcs[i] != NULL; i++) {
        if (strcmp(command, commands_list[i]) == 0)
            return command_funcs[i];
    }
    return NULL;
}

void client_send(client_t *client, char *message)
{
    char buffer[1024] = {0};

    sprintf(buffer, "%s\r\n", message);
    write(client->sd, buffer, strlen(buffer));
}

void client_read(client_t *client)
{
    char *buffer = client->buffer;
    char **command = buffer_split(buffer);
    command_func_t command_func = NULL;

    if (command[0] == NULL)
        return;
    command_func = client_find(command);
    if (command_func == NULL)
        return client_send(client, "500 Unknown command.");
    command_func(client, command);
}
