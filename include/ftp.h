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

//////////////////////// client ////////////////////////

enum client_state {
    NOT_LOGGED = 0,
    LOGGED
};

typedef struct client_s {
    char buffer[1024];
    int sd;
    char current_path[1024];
    int total_bytes;
    char *username;
    enum client_state state;
} client_t;

static const char *_250 = "250 Requested file action okay, completed.";
static const char *_120 = "Service ready in nnn minutes.";
static const char *_125 = "Data connection already open; transfer starting.";
static const char *_150 = "File status okay; about to open data connection.";
static const char *_200 = "Command okay.";
static const char *_214 = "Help message.";
static const char *_220 = "Service ready for new user.";
static const char *_221 = "Service closing control connection. \
Logged out if appropriate.";
static const char *_226 = "Closing data connection.\nRequested file \
action successful (for example, file transfer or file abort).";
static const char *_227 = "Entering Passive Mode (h1,h2,h3,h4,p1,p2).";
static const char *_230 = "User logged in, proceed.";
static const char *_331 = "User name okay, need password.";
static const char *_332 = "Need account for login.";
static const char *_530 = "Login with USER and PASS first.";
    #define _257(path) "257 " path " is the current directory."

typedef void (*handler_t)(client_t *client, char **command);

void client_read(client_t *client);
void client_send(client_t *client, char *message);
handler_t client_find(char **buffer_tab);

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

//////////////////////// clients list ////////////////////////

typedef client_t *client_list_t;

client_list_t clients_list_get(void);
int clients_list_fill_fd_set(int server_fd, fd_set *readfds);
void clients_list_add(int new_socket);
void clients_list_read(fd_set *readfds);


#endif /* !FTP_H_ */
