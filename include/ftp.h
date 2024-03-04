/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** ftp
*/

#ifndef FTP_H_
    #define FTP_H_


    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <stdlib.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>

typedef struct addrinfo {
    struct sockaddr_in addr;
    socklen_t len;
    struct sockaddr* ptr;
} addrinfo_t;

    #define SHOW_IP(ADDR) inet_ntoa(ADDR->addr.sin_addr)
    #define SHOW_PORT(ADDR) ntohs(ADDR->addr.sin_port)
    #define MAX_CLIENTS 30


int bind_max_sd(int server_fd, fd_set *readfds);
void add_client(int new_socket);
int *get_available_connections(void);
void close_unused_sockets(fd_set *readfds, addrinfo_t *addr);
addrinfo_t *create_sock_addr(void);
void bind_sock_addr(int server_fd);


#endif /* !FTP_H_ */
