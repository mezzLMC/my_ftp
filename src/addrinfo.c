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

void bind_sock_addr(int server_fd)
{
    addrinfo_t *addr = create_sock_addr();

    addr->addr.sin_family = AF_INET;
    addr->addr.sin_addr.s_addr = INADDR_ANY;
    addr->addr.sin_port = htons(4242);
    bind(server_fd, addr->ptr, addr->len);
    free(addr);
}
