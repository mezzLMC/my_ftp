/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** client
*/

#include "ftp.h"
#include "commands.h"

handler_t client_find(char **buffer_tab)
{
    int i = 0;
    char *command = buffer_tab[0];

    for (i = 0; command_funcs[i] != NULL; i++) {
        if (strcmp(command, commands_list[i]) == 0)
            return command_funcs[i];
    }
    return NULL;
}

void client_send(client_t *client, errormsg message)
{
    char buffer[1024] = {0};

    sprintf(buffer, "%s\r\n", message);
    write(client->sd, buffer, strlen(buffer));
}

void client_read(client_t *client)
{
    char *buffer = client->buffer;
    char *buffer_copy = strdup(buffer);
    char **command = buffer_split(buffer_copy);
    handler_t handler = NULL;

    if (command[0] == NULL)
        return client_send(client, _500);
    handler = client_find(command);
    if (handler == NULL)
        return client_send(client, _500);
    chdir(client->current_path);
    command_login_wrap(handler, client, command);
}

void client_watch_subconnection(client_t *client)
{
    sub_connection_t *sub_co = client->sub_connection;
    addrinfo_t *addr = create_sock_addr();
    int new_socket = 0;

    if (sub_co) {
        new_socket = accept(sub_co->data_fd, addr->ptr, &(addr->len));
        if (new_socket < 0) {
            return;
        }
        sub_co->new_socket = new_socket;
    }
}
