/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** data_transfer
*/

#include "ftp.h"
#include <sys/wait.h>
#include <errno.h>


static void sub_connection_fill_buffer(int fd, sub_connection_t *sub_co)
{
    int n = 0;

    n = read(fd, sub_co->buffer, 4096);
    sub_co->buffer[n - 1] = '\0';
}

void client_close_sub_connection(client_t *client, errormsg message)
{
    sub_connection_t *sub_co = client->sub_connection;

    if (!client->sub_connection)
        return;
    if (client->transfer_type == PASSIVE)
        close(sub_co->data_fd);
    if (sub_co->new_socket > 0)
        close(sub_co->new_socket);
    client->sub_connection = NULL;
    free(sub_co);
    if (message)
        client_send(client, message);
}

static void sub_connections_flush_buffer(client_t *client)
{
    sub_connection_t *sub_co = client->sub_connection;

    if (sub_co && sub_co->new_socket > 0 && sub_co->buffer[0] != '\0') {
        dprintf(sub_co->new_socket, "%s\r\n", sub_co->buffer);
        client_send(client, _226);
    }
    client->transfer_type = NONE;
}

static void child_transfer_data(int fd[2], handler_t handler,
    client_t *client, char **command)
{
    close(client->sub_connection->new_socket);
    dup2(fd[1], 1);
    close(fd[0]);
    handler(client, command);
    exit(0);
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
    close(sub_co->new_socket);
    client->transfer_status = TRANSFERED;
    exit(0);
}

static void data_transfer_ensure_connection(client_t *client)
{
    int connected = 0;
    sub_connection_t *sub_co = client->sub_connection;

    if (client->transfer_type == PASSIVE) {
        client_send(client, sub_co->new_socket < 0 ? _150 : _125);
        while (sub_co->new_socket < 0)
            sub_co->new_socket = accept(sub_co->data_fd, NULL, NULL);
    }
    if (client->transfer_type == ACTIVE) {
        client_send(client, _150);
        connected = connect(sub_co->new_socket,
        (struct sockaddr *)&sub_co->addr_data, sizeof(sub_co->addr_data));
        if (connected < 0)
            client_send(client, _425);
    }
}

static void handle_sub_process(handler_t data_handler, client_t *client,
    char **command)
{
    int fd[2];
    pid_t pid;

    data_transfer_ensure_connection(client);
    pipe(fd);
    pid = fork();
    if (pid == 0)
        child_transfer_data(fd, data_handler, client, command);
    else
        parent_receive_transfer(fd, pid, client);
}

void sub_connection_execute(client_t *client, char **command,
    handler_t data_handler)
{
    int pid = 0;
    sub_connection_t *sub_co = client->sub_connection;

    client->transfer_status = TRANSFERED;
    pid = fork();
    if (pid == 0)
        handle_sub_process(data_handler, client, command);
    else if (client->transfer_type == PASSIVE && sub_co->new_socket > 0)
        close(sub_co->new_socket);
}
