/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** utils
*/

#include "ftp.h"

char **buffer_split(char *buffer)
{
    char **res = malloc(sizeof(char *) * 1024);
    char *token = strtok(buffer, " ");
    int i = 0;

    for (i = 0; token != NULL; i++) {
        res[i] = token;
        token = strtok(NULL, " ");
    }
    res[i] = NULL;
    return res;
}
