/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** main
*/

#include "ftp.h"


int main(void)
{
    int server_fd = server_create();
    server_run(server_fd);
    return 0;
}
