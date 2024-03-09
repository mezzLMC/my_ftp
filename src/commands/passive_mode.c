/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** passive_mode
*/

#include "ftp.h"


static void create_passive_socket(client_t *client, int newPort)
{
    sub_connection_t *sub_connection = malloc(sizeof(sub_connection_t));

    memset(sub_connection, 0, sizeof(sub_connection_t));
    sub_connection->port = newPort;
    sub_connection->data_fd = server_create(newPort);
    sub_connection->new_socket = -1;
    client->sub_connection = sub_connection;
    listen(sub_connection->data_fd, 1);
}

void command_pasv(client_t *client, __attribute__((unused)) char **command)
{
    int port = server_get_port(0);
    int newPort = 10 * 256 + port;
    char buffer[1024] = {0};

    sprintf(buffer, "227 Entering Passive Mode (127,0,0,1,%d,%d)", 10, port);
    client_send(client, buffer);
    server_get_port(port + 1);
    create_passive_socket(client, newPort);
    client->transfer_type = PASSIVE;
}
