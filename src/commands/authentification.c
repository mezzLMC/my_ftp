/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** authentification
*/

#include "ftp.h"

void command_username(client_t *client, char **command)
{
    if (client->state == LOGGED)
        return client_send(client, _230);
    if (command[1] == NULL || strcmp(command[1], "") == 0)
        return client_send(client, "332 User name not specified.");
    client->username = strdup(command[1]);
    client_send(client, "331 User name okay, need password.");
}

void command_password(client_t *client, char **command)
{
    if (client->username == NULL)
        return client_send(client, _332);
    if (strcmp(client->username, "Anonymous") != 0)
        return client_send(client, _530);
    if (command[1] == NULL || strcmp(command[1], "") == 0) {
        client->state = LOGGED;
        return client_send(client, _230);
    }
}

void command_quit(client_t *client, __attribute__((unused)) char **command)
{
    client_send(client, "221 Service closing control connection.");
    close(client->sd);
    client_close_sub_connection(client, NULL);
    memset(client, 0, sizeof(client_t));
}
