/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** commands
*/

#include "ftp.h"
#include "commands.h"

static int need_login(handler_t handlr)
{
    handler_t whitelist[] = {
        command_username, command_password, command_quit, NULL
    };

    for (int i = 0; whitelist[i] != NULL; i++)
        if (whitelist[i] == handlr)
            return 0;
    return 1;
}

static int need_sub_connection(handler_t handlr)
{
    handler_t whitelist[] = {
        command_list, command_retr, command_stor, command_dele, NULL
    };

    for (int i = 0; whitelist[i] != NULL; i++)
        if (whitelist[i] == handlr)
            return 1;
    return 0;
}

void command_login_wrap(handler_t handler, client_t *client, char **command)
{
    if (need_login(handler) && client->state == NOT_LOGGED)
        return client_send(client, _530);
    if (need_sub_connection(handler) && client->transfer_type == NONE)
        return client_send(client, _425);
    return handler(client, command);
}

void command_noop(client_t *client, __attribute__((unused)) char **command)
{
    client_send(client, _200);
}

void command_help(client_t *client, __attribute__((unused)) char **command)
{
    client_send(client, _214);
}
