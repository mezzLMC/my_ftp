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

void sub_connections_flush_buffer(void)
{
    client_list_t clients = clients_list_get();
    sub_connection_t *sub_co = NULL;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        sub_co = clients[i].sub_connection;
        if (sub_co && sub_co->new_socket > 0 && sub_co->buffer[0] != '\0') {
            dprintf(sub_co->new_socket, "%s\r\n", sub_co->buffer);
            sub_co->buffer[0] = '\0';
            clients[i].transfer_type = NONE;
            close(sub_co->new_socket);
            close(sub_co->data_fd);
            clients[i].sub_connection = NULL;
            free(sub_co);
            client_send(&clients[i], _226);
        }
    }
}

static void child_list(int fd[2])
{
    dup2(fd[1], 1);
    close(fd[0]);
    execlp("ls", "ls", "-l", NULL);
}

static void parent_list(int fd[2], pid_t pid, sub_connection_t *sub_co)
{
    int status;

    waitpid(pid, &status, 0);
    close(fd[1]);
    sub_connection_fill_buffer(fd[0], sub_co);
    close(fd[0]);
}

void command_list(client_t *client, __attribute__((unused)) char **command)
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
            child_list(fd);
        else
            parent_list(fd, pid, sub_co);
    } else
        dprintf(client->sd, "425 Use PASV or PORT first.\r\n");
}
