/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** socket_list
*/

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "ftp.h"

client_list_t clients_list_get(void)
{
    static client_list_t socket = NULL;

    if (socket == NULL) {
        socket = malloc(sizeof(client_t) * MAX_CLIENTS);
        memset(socket, 0, sizeof(client_t) * MAX_CLIENTS);
    }
    return socket;
}

int clients_list_fill_fd_set(int server_fd, fd_set *readfds)
{
    int max_sd = server_fd;
    client_list_t client_list = clients_list_get();

    FD_ZERO(readfds);
    FD_SET(server_fd, readfds);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i].sd > 0)
            FD_SET(client_list[i].sd, readfds);
        if (client_list[i].sd > max_sd)
            max_sd = client_list[i].sd;
    }
    return max_sd;
}

void clients_list_add(int new_socket)
{
    client_list_t client_list = clients_list_get();
    int flags = fcntl(new_socket, F_GETFL, 0);

    fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i].sd == 0) {
            client_list[i].sd = new_socket;
            printf("Adding to list of sockets as %d\n", i);
            break;
        }
    }
}

void clients_list_read(fd_set *readfds)
{
    client_list_t client_list = clients_list_get();
    client_t *client;
    int valread;
    char c;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client = &client_list[i];
        if (!(FD_ISSET(client->sd, readfds)))
            continue;
        valread = read(client->sd, &c, 1);

        if (valread == -1)
            continue;
        if (valread == 0) {
            close(client->sd);
            client_list[i] = (client_t) {0};
            continue;
        }
        if (c == '\n' || c == '\r') {
            client->buffer[client->total_bytes] = '\0';
            client->total_bytes = 0;
            dprintf(1, "\ncommand was %s\n", client->buffer);
            write(client->sd, "200 Command okay.\n", 18);
        } else {
            client->buffer[client->total_bytes] = c;
            client->total_bytes++;
        }
        write(1, &c, 1);
    }
}
