/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** addrinfo
*/

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


#include "ftp.h"

addrinfo_t *create_sock_addr(void)
{
    addrinfo_t *addr = malloc(sizeof(addrinfo_t));

    addr->len = sizeof(addr->addr);
    addr->ptr = (struct sockaddr*)&addr->addr;
    return addr;
}

void bind_sock_addr(int server_fd, int port)
{
    addrinfo_t *addr = create_sock_addr();
    int bind_ret;

    addr->addr.sin_family = AF_INET;
    addr->addr.sin_addr.s_addr = INADDR_ANY;
    addr->addr.sin_port = htons(port);
    bind_ret = bind(server_fd, addr->ptr, addr->len);
    if (bind_ret < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    free(addr);
}
