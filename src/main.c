/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** main
*/

#include "ftp.h"

bool validate_port(char *port)
{
    for (int i = 0; port[i] != '\0'; i++) {
        if (port[i] < '0' || port[i] > '9')
            return false;
    }
    return true;
}

int main(int ac, char **av)
{
    int server_fd;
    int port;

    if (ac != 3 || chdir(av[2]) == -1 || !validate_port(av[1])) {
        write(2, "Usage: ./bsFTP port path\n", 25);
        return 84;
    }
    port = atoi(av[1]);
    server_fd = server_create(port);
    server_run(server_fd, av[2]);
    return 0;
}
