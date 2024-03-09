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

    if (server_fd == 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind_sock_addr(server_fd, port);
    flags = fcntl(server_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    if (fcntl(server_fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

char *server_get_root(char *path)
{
    char buffer[1024] = {0};
    static char *_path = NULL;

    if (path != NULL) {
        getcwd(buffer, 1024);
        _path = strdup(buffer);
    }
    return _path;
}

void server_accept(int server_fd, fd_set *readfds, addrinfo_t *addr)
{
    struct timeval tv = {0, 0};
    int new_socket;
    int activy = 0;
    client_list_t clients = clients_list_get();

    for (int i = 0; i < MAX_CLIENTS; i++)
        client_watch_subconnection(&clients[i]);
    if (FD_ISSET(server_fd, readfds)) {
        new_socket = accept(server_fd, addr->ptr, &(addr->len));
        if (new_socket < 0)
            return;
        activy = select(new_socket + 1, readfds, NULL, NULL, &tv);
        if (activy < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }
        clients_list_add(new_socket);
    }
}

void server_run(int server_fd, char *root)
{
    fd_set readfds;
    addrinfo_t *addr = create_sock_addr();

    server_get_root(root);
    listen(server_fd, MAX_CLIENTS);
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfds);
    while (1) {
        server_accept(server_fd, &readfds, addr);
        sub_connections_flush_buffer();
        clients_list_fill_fd_set(server_fd, &readfds);
        clients_list_read(&readfds);
    }
    free(addr);
}
