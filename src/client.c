/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** client
*/

#include "ftp.h"
#include "commands.h"

handler_t client_find(char **buffer_tab)
{
    int i = 0;
    char *command = buffer_tab[0];

    for (i = 0; command_funcs[i] != NULL; i++) {
        if (strcmp(command, commands_list[i]) == 0)
            return command_funcs[i];
    }
    return NULL;
}

void client_send(client_t *client, errormsg message)
{
    char buffer[1024] = {0};

    sprintf(buffer, "%s\r\n", message);
    write(client->sd, buffer, strlen(buffer));
}

void client_read(client_t *client)
{
    char *buffer = client->buffer;
    char **command = buffer_split(buffer);
    handler_t handler = NULL;

    chdir(client->current_path);
    if (command[0] == NULL)
        return;
    handler = client_find(command);
    if (handler == NULL)
        return client_send(client, "500 Unknown command.");
    command_login_wrap(handler, client, command);
}
