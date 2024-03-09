/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** data_transfer
*/

#include "ftp.h"
#include <sys/wait.h>


static void sub_connection_fill_buffer(int fd, sub_connection_t *sub_co)
{
    int n = 0;

    n = read(fd, sub_co->buffer, 4096);
    sub_co->buffer[n - 1] = '\0';
}

void client_close_sub_connection(client_t *client)
{
    sub_connection_t *sub_co = client->sub_connection;

    if (!client->sub_connection)
        return;
    client->transfer_type = NONE;
    close(sub_co->new_socket);
    close(sub_co->data_fd);
    client->sub_connection = NULL;
    free(sub_co);
    client_send(client, _226);
}

void sub_connections_flush_buffer(client_t *client)
{
    sub_connection_t *sub_co = client->sub_connection;

    if (sub_co && sub_co->new_socket > 0 && sub_co->buffer[0] != '\0') {
        dprintf(sub_co->new_socket, "%s\r\n", sub_co->buffer);
        client_close_sub_connection(client);
    }
}

static void child_transfer_data(int fd[2], handler_t handler,
    client_t *client, char **command)
{
    dup2(fd[1], 1);
    close(fd[0]);
    handler(client, command);
}

static void parent_receive_transfer(int fd[2], pid_t pid, client_t *client)
{
    int status;
    sub_connection_t *sub_co = client->sub_connection;

    waitpid(pid, &status, 0);
    close(fd[1]);
    sub_connection_fill_buffer(fd[0], sub_co);
    close(fd[0]);
    sub_connections_flush_buffer(client);
}

void sub_connection_execute(client_t *client, char **command,
    handler_t data_handler)
{
    int pid = 0;
    sub_connection_t *sub_co = client->sub_connection;
    int fd[2];

    if (client->transfer_type == PASSIVE) {
        client_send(client, sub_co->new_socket < 0 ? _150 : _125);
        while (sub_co->new_socket < 0)
            sub_co->new_socket = accept(sub_co->data_fd, NULL, NULL);
        pipe(fd);
        pid = fork();
        if (pid == 0)
            child_transfer_data(fd, data_handler, client, command);
        else
            parent_receive_transfer(fd, pid, client);
    } else
        dprintf(client->sd, "425 Use PASV or PORT first.\r\n");
}
