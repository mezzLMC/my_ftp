/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** main
*/

#include "ftp.h"

int server_create(int port)
{
    int opt = 1;
    int flags;
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind_sock_addr(server_fd, port);
    flags = fcntl(server_fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(server_fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void server_accept(int server_fd, fd_set *readfds, addrinfo_t *addr)
{
    struct timeval tv = {0, 0};
    int new_socket;
    int activy = 0;

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
        clients_list_add(new_socket);
    }
}

void server_run(int server_fd)
{
    fd_set readfds;
    addrinfo_t *addr = create_sock_addr();

    listen(server_fd, MAX_CLIENTS);
    while (1) {
        server_accept(server_fd, &readfds, addr);
        clients_list_fill_fd_set(server_fd, &readfds);
        clients_list_read(&readfds);
    }
    free(addr);
}
