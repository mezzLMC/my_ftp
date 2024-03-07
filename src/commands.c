/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** commands
*/

#include "ftp.h"
#include "commands.h"

void command_login_wrap(handler_t handlr, client_t *client, char **command)
{
    handler_t whitelist[] = {
        command_username, command_password, command_quit, NULL
    };

    for (int i = 0; whitelist[i] != NULL; i++)
        if (whitelist[i] == handlr)
            return handlr(client, command);
    if (client->state == NOT_LOGGED)
        return client_send(client, _530);
    else
        return handlr(client, command);
}

void command_noop(client_t *client, __attribute__((unused)) char **command)
{
    client_send(client, _200);
}

void command_help(client_t *client, __attribute__((unused)) char **command)
{
    client_send(client, _214);
}
