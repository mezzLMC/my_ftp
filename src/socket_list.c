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

int *get_available_connections(void)
{
    static int *socket = NULL;

    if (socket == NULL) {
        socket = malloc(sizeof(int) * MAX_CLIENTS);
        for (int i = 0; i < MAX_CLIENTS; i++)
            socket[i] = 0;
    }
    return socket;
}

int bind_max_sd(int server_fd, fd_set *readfds)
{
    int max_sd = server_fd;
    int *client_socket = get_available_connections();

    FD_ZERO(readfds);
    FD_SET(server_fd, readfds);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_socket[i] > 0)
            FD_SET(client_socket[i], readfds);
        if (client_socket[i] > max_sd)
            max_sd = client_socket[i];
    }
    return max_sd;
}

void add_client(int new_socket)
{
    int *client_socket = get_available_connections();
    int flags = fcntl(new_socket, F_GETFL, 0);

    fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_socket[i] == 0) {
            client_socket[i] = new_socket;
            printf("Adding to list of sockets as %d\n", i);
            break;
        }
    }
}

void close_unused_sockets(fd_set *readfds, addrinfo_t *addr)
{
    int *client_socket = get_available_connections();
    char buffer[1024] = {0};
    int valread;
    int sd;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        sd = client_socket[i];
        if (!(FD_ISSET(sd, readfds)))
            continue;
        valread = read(sd, buffer, 1);
        if (valread == 0) {
            getpeername(sd, addr->ptr, &(addr->len));
            printf("disconnected %s:%d\n", SHOW_IP(addr), SHOW_PORT(addr));
            close(sd);
            client_socket[i] = 0;
        } else {
            buffer[valread] = '\0';
            write(sd, buffer, strlen(buffer));
        }
    }
}
