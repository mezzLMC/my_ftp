/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** data_transfer
*/

#include "ftp.h"

void execute_list(client_t *client, char **command)
{
    (void)command;
    (void)client;
    execlp("ls", "ls", "-l", NULL);
}

void command_list(client_t *client, __attribute__((unused)) char **command)
{
    sub_connection_execute(client, command, execute_list);
}

void execute_retr(client_t *client, char **command)
{
    (void)command;
    (void)client;
    execlp("cat", "cat", command[1], NULL);
}

void command_retr(client_t *client, char **command)
{
    if (command[1] == NULL || strcmp(command[1], "") == 0) {
        client_close_sub_connection(client);
        return client_send(client, "501 No file name specified.");
    }
    if (access(command[1], F_OK) == -1) {
        client_close_sub_connection(client);
        return client_send(client, "550 File not found or access denied.");
    }
    sub_connection_execute(client, command, execute_retr);
}
