/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** commands
*/

#ifndef COMMAND_H_
    #define COMMAND_H_

    #include <stddef.h>
    #include "ftp.h"

__attribute__((unused)) static char *commands_list[] = {
    "USER",
    "PASS",
    "QUIT",
    "PWD",
    "CWD",
    "CDUP",
    "NOOP",
    "HELP",
    "PASV",
    "LIST",
    "RETR",
    "DELE",
    "PORT",
    "STOR",
    NULL
};

void command_username(client_t *client, char **command);
void command_password(client_t *client, char **command);
void command_quit(client_t *client, char **command);
void command_cwd(client_t *client, char **command);
void command_cdup(client_t *client, char **command);
void command_dele(client_t *client, char **command);
void command_pwd(client_t *client, char **command);
void command_pasv(client_t *client, char **command);
void command_port(client_t *client, char **command);
void command_help(client_t *client, char **command);
void command_noop(client_t *client, char **command);
void command_retr(client_t *client, char **command);
void command_stor(client_t *client, char **command);
void command_list(client_t *client, char **command);

__attribute__((unused)) static handler_t command_funcs[] = {
    command_username,
    command_password,
    command_quit,
    command_pwd,
    command_cwd,
    command_cdup,
    command_noop,
    command_help,
    command_pasv,
    command_list,
    command_retr,
    // command_dele,
    // command_port,
    // command_stor,
    NULL
};

void command_login_wrap(handler_t handlr, client_t *client, char **command);


#endif /* !command_H_ */
