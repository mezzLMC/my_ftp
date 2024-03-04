/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** ftp
*/

#ifndef FTP_H_
    #define FTP_H_

    #include <stdio.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/select.h>
    #include <string.h>



//////////////////////// client ////////////////////////

typedef struct client_s {
    char buffer[1024];
    int sd;
    char* current_path[1024];
    int total_bytes;
} client_t;

void client_read(client_t *client);


//////////////////////// utils ////////////////////////

typedef struct addrinfo {
    struct sockaddr_in addr;
    socklen_t len;
    struct sockaddr* ptr;
} addrinfo_t; // just to make the code more readable and avoir casting everywhere


addrinfo_t *create_sock_addr(void);
void bind_sock_addr(int server_fd);

    #define SHOW_IP(ADDR) inet_ntoa(ADDR->addr.sin_addr)
    #define SHOW_PORT(ADDR) ntohs(ADDR->addr.sin_port)
    #define MAX_CLIENTS 30


//////////////////////// server ////////////////////////

void server_run(int server_fd);
int server_create(void);
void server_accept(int server_fd, fd_set *readfds, addrinfo_t *addr);

//////////////////////// clients list ////////////////////////

typedef client_t* client_list_t;

client_list_t clients_list_get(void);
int clients_list_fill_fd_set(int server_fd, fd_set *readfds);
void clients_list_add(int new_socket);
void clients_list_read(fd_set *readfds);


#endif /* !FTP_H_ */
