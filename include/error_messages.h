/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** error_messages
*/

#ifndef ERROR_MESSAGES_H_
#define ERROR_MESSAGES_H_

    #define _257(path)  "257 " path " is the current directory."

typedef __attribute__((unused)) const char *errormsg;

static errormsg _250 = "250 Requested file action okay, completed.";
static errormsg _120 = "120 Service ready in nnn minutes.";
static errormsg _125 = "125 Data connection already open; transfer starting.";
static errormsg _150 = "150 File status okay; about to open data connection.";
static errormsg _200 = "200 Command okay.";
static errormsg _214 = "214 Help message.";
static errormsg _220 = "220 Service ready for new user.";
static errormsg _221 = "221 Service closing control connection. \
Logged out if appropriate.";
static errormsg _226 = "226 Closing data connection.\nRequested file \
action successful (for example, file transfer or file abort).";
static errormsg _227 = "227 Entering Passive Mode (h1,h2,h3,h4,p1,p2).";
static errormsg _230 = "230 User logged in, proceed.";
static errormsg _331 = "331 User name okay, need password.";
static errormsg _332 = "332 Need account for login.";
static errormsg _530 = "530 Login with USER and PASS first.";
static errormsg _550 = "550 Failed to change directory.";


#endif /* !ERROR_MESSAGES_H_ */
