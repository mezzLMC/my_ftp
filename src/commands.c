/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** commands
*/

#include "ftp.h"

void command_username(client_t *client, char **command)
{
    if (client->state == LOGGED)
        return client_send(client, "501 Disconnect first to re-login.");
    if (command[1] == NULL || strcmp(command[1], "") == 0)
        return client_send(client, "501 User name not specified.");
    client->username = strdup(command[1]);
    client_send(client, "331 User name okay, need password.");
}

void command_password(client_t *client, char **command)
{
    if (client->username == NULL)
        return client_send(client, "503 Login with USER first.");
    if (strcmp(client->username, "Anonymous") != 0)
        return client_send(client, "Authentication rejected");
    if (command[1] == NULL || strcmp(command[1], "") == 0) {
        client->state = LOGGED;
        return client_send(client, "230 User logged in, proceed.");
    }
}

void command_quit(client_t *client, __attribute__((unused)) char **command)
{
    client_send(client, "221 Service closing control connection.");
    close(client->sd);
    memset(client, 0, sizeof(client_t));
}
