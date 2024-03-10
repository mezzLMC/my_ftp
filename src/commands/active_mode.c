/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** active_mode
*/

#include "ftp.h"

static void create_active_mode(client_t *client, in_addr_t ip, int port)
{
    sub_connection_t *sub_connection = malloc(sizeof(sub_connection_t));

    memset(sub_connection, 0, sizeof(sub_connection_t));
    sub_connection->new_socket = socket(AF_INET, SOCK_STREAM, 0);
    client->sub_connection = sub_connection;
    client->transfer_type = ACTIVE;
    sub_connection->addr_data.sin_addr.s_addr = ip;
    sub_connection->addr_data.sin_port = htons(port);
    sub_connection->addr_data.sin_family = AF_INET;
    client_send(client, "200 PORT command successful.");
}

static int valide_number(char *ip)
{
    for (int i = 0; ip[i] != '\0'; i++)
        if (ip[i] < '0' || ip[i] > '9')
            return 1;
    return 0;
}

static int validate_port(char **ip)
{
    for (int i = 0; ip[i] != NULL; i++)
        if (valide_number(ip[i]) == 1)
            return 1;
    return 0;
}

void command_port(client_t *client, char **command)
{
    char **ip_tab;
    int port;
    char str_ip[1024] = {0};
    in_addr_t ip;

    if (command[1] == NULL)
        return client_send(client, _501);
    ip_tab = buffer_split_sep(command[1], ",");
    if (tablen(ip_tab) != 6 || validate_port(ip_tab) == 1)
        return client_send(client, _501);
    sprintf(str_ip, "%s.%s.%s.%s", ip_tab[0], ip_tab[1], ip_tab[2], ip_tab[3]);
    ip = inet_addr(str_ip);
    port = atoi(ip_tab[4]) * 256 + atoi(ip_tab[5]);
    create_active_mode(client, ip, port);
}
