/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** data_transfer
*/

#include "ftp.h"

static void execute_list(client_t *client, char **command)
{
    (void)command;
    (void)client;
    execlp("ls", "ls", "-l", NULL);
}

static void execute_retr(client_t *client, char **command)
{
    (void)command;
    (void)client;
    execlp("cat", "cat", command[1], NULL);
}

static void execute_stor(client_t *client, char **command)
{
    int fd = open(command[1], O_CREAT | O_WRONLY, 0644);
    char buffer[1024] = {0};
    int n;

    while (1) {
        n = read(client->sub_connection->new_socket, buffer, 1024);
        if (n <= 0)
            break;
        write(fd, buffer, n);
    }
    dprintf(1, "ouais c bon\n");
}

static void execute_dele(client_t *client, char **command)
{
    if (remove(command[1]) == 0)
        client_send(client, "250 Requested file action okay, completed.");
    else
        client_send(client, "550 Failed to delete file.");
}

void command_list(client_t *client, __attribute__((unused)) char **command)
{
    sub_connection_execute(client, command, execute_list);
}

void command_retr(client_t *client, char **command)
{
    if (command[1] == NULL || strcmp(command[1], "") == 0) {
        client_close_sub_connection(client, NULL);
        return client_send(client, "501 No file name specified.");
    }
    if (access(command[1], F_OK) == -1) {
        client_close_sub_connection(client, NULL);
        return client_send(client, "550 File not found or access denied.");
    }
    sub_connection_execute(client, command, execute_retr);
}

void command_stor(client_t *client, char **command)
{
    if (command[1] == NULL || strcmp(command[1], "") == 0) {
        client_close_sub_connection(client, NULL);
        return client_send(client, "501 No file name specified.");
    }
    sub_connection_execute(client, command, execute_stor);
}

void command_dele(client_t *client, char **command)
{
    char *root = server_get_root(NULL);
    char buffer[1024] = {0};

    if (command[1] == NULL || strcmp(command[1], "") == 0) {
        client_close_sub_connection(client, NULL);
        return client_send(client, "501 No file name specified.");
    }
    if (command[1][0] == '/') {
        strcat(buffer, root);
        strcat(buffer, command[1]);
        command[1] = strdup(buffer);
    }
    sub_connection_execute(client, command, execute_dele);
}
