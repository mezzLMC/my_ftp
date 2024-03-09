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
    #include <stdbool.h>
    #include "error_messages.h"

//////////////////////// client ////////////////////////

enum client_state {
    NOT_LOGGED = 0,
    LOGGED
};

enum data_transfer_type {
    NONE = 0,
    ACTIVE,
    PASSIVE
};

typedef struct subconnection {
    int data_fd;
    int port;
    int new_socket;
    char buffer[4096];
} sub_connection_t;

typedef struct client_s {
    char buffer[1024];
    int sd;
    char current_path[1024];
    int total_bytes;
    char *username;
    enum client_state state;
    enum data_transfer_type transfer_type;
    sub_connection_t *sub_connection;
    int data_fd;
} client_t;


typedef void (*handler_t)(client_t *client, char **command);

void client_read(client_t *client);
void client_send(client_t *client, errormsg msg);
handler_t client_find(char **buffer_tab);
void client_watch_subconnection(client_t *client);
void client_close_sub_connection(client_t *client, errormsg message);
void sub_connection_execute(client_t *client,
    char **command, handler_t data_handler);

//////////////////////// utils ////////////////////////

typedef struct addrinfo {
    struct sockaddr_in addr;
    socklen_t len;
    struct sockaddr *ptr;
} addrinfo_t;
// just to make the code more readable and avoir casting everywhere


addrinfo_t *create_sock_addr(void);
void bind_sock_addr(int server_fd, int port);
char **buffer_split(char *buffer);


    #define SHOW_IP(ADDR) inet_ntoa(ADDR->addr.sin_addr)
    #define SHOW_PORT(ADDR) ntohs(ADDR->addr.sin_port)
    #define MAX_CLIENTS 30


//////////////////////// server ////////////////////////

void server_run(int server_fd, char *root);
int server_create(int port);
void server_accept(int server_fd, fd_set *readfds, addrinfo_t *addr);
char *server_get_root(char *path);
int server_get_port(int port);

//////////////////////// clients list ////////////////////////

typedef client_t *client_list_t;

client_list_t clients_list_get(void);
int clients_list_fill_fd_set(int server_fd, fd_set *readfds);
void clients_list_add(int new_socket);
void clients_list_read(fd_set *readfds);


#endif /* !FTP_H_ */
