/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** commands
*/

#ifndef command_H_
#define command_H_

#include <stddef.h>
#include "ftp.h"

static char* commands_list[] = {
    "USER",
    "PASS",
    "QUIT",
    "PWD",
    "CWD",
    "CDUP",
    "DELE",
    "PASV",
    "PORT",
    "HELP",
    "NOOP",
    "RETR",
    "STOR",
    "LIST",
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

static command_func_t command_funcs[] = {
    command_username,
    command_password,
    command_quit,
    command_pwd,
    command_cwd,
    // command_cdup,
    // command_dele,
    // command_pasv,
    // command_port,
    // command_help,
    // command_noop,
    // command_retr,
    // command_stor,
    // command_list,
    NULL
};


#endif /* !command_H_ */
