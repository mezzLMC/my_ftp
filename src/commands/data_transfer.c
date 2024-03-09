/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** data_transfer
*/

#include "ftp.h"
#include <sys/wait.h>

void command_list(client_t *client, __attribute__((unused)) char **command)
{
    int pid = 0;
    int status = 0;

    if (client->transfer_type == PASSIVE) {
        client_send(client, "150 Here comes the directory listing.");
        pid = fork();
        if (pid == 0) {
            dup2(client->sub_connection->new_socket, 1);
            dup2(client->sub_connection->new_socket, 2);
            execlp("ls", "ls", "-l", client->current_path, NULL);
            exit(0);
        }
        waitpid(pid, &status, 0);
        dprintf(client->sd, "226 Directory send OK.\r\n");
    } else
        dprintf(client->sd, "425 Use PASV or PORT first.\r\n");
}
