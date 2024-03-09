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
    char *root = server_get_root(NULL);

    fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i].sd == 0) {
            client_list[i].sd = new_socket;
            strcpy(client_list[i].current_path, root);
            client_send(&client_list[i], "220 Service ready for new user.");
            break;
        }
    }
}

void client_execute(client_t *client, char c)
{
    char *buffer = client->buffer;

    buffer[client->total_bytes] = c;
    client->total_bytes++;
    buffer[client->total_bytes] = '\0';
    if (strstr(buffer, "\r\n") != NULL) {
        buffer[strlen(buffer) - 2] = '\0';
        client_read(client);
        client->total_bytes = 0;
        client->buffer[0] = '\0';
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
            client_close_sub_connection(client);
            memset(client, 0, sizeof(client_t));
            continue;
        }
        client_execute(client, c);
    }
}
