/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** main
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#include <stdlib.h>
#include <sys/select.h>
#include "ftp.h"

int create_server(void)
{
    int opt = 1;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind_sock_addr(server_fd);
    return server_fd;
}

void accept_new_connection(int server_fd, fd_set *readfds, addrinfo_t *addr)
{
    struct timeval tv;
    int new_socket;
    int activy = 0;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    if (FD_ISSET(server_fd, readfds)) {
        new_socket = accept(server_fd, addr->ptr, &(addr->len));
        if (new_socket < 0) {
            return;
        }
        activy = select(new_socket + 1, readfds, NULL, NULL, &tv);
        if (activy < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }
        add_client(new_socket);
    }
}

void select_server(int server_fd)
{
    fd_set readfds;
    addrinfo_t *addr = create_sock_addr();
    int flags = fcntl(server_fd, F_GETFL, 0);

    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(server_fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    listen(server_fd, MAX_CLIENTS);
    while (1) {
        bind_max_sd(server_fd, &readfds);
        accept_new_connection(server_fd, &readfds, addr);
        close_unused_sockets(&readfds, addr);
    }
}

int main(void)
{
    int server_fd = create_server();

    select_server(server_fd);
    return 0;
}
