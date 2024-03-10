/*
** EPITECH PROJECT, 2024
** bsFTP
** File description:
** error_messages
*/

#ifndef ERROR_MESSAGES_H_
    #define ERROR_MESSAGES_H_

    #define _257(path)  "257 \"" path "\" is the current directory."

typedef const char *errormsg;

    #define _250    "250 Requested file action okay, completed."

    #define _120    "120 Service ready in nnn minutes."

    #define _125    "125 Data connection already open. transfer starting."

    #define _150    "150 File status okay. about to open data connection."

    #define _200    "200 Command okay."

    #define _214    "214 Help message."

    #define _220    "220 Service ready for new user."

    #define _221    "221 Service closing control connection. Logged out."

    #define _226    "226 Closing data connection. Requested file successful"

    #define _227    "227 Entering Passive Mode (h1,h2,h3,h4,p1,p2)."

    #define _230    "230 User logged in, proceed."

    #define _256    "256 Directory send OK."

    #define _331    "331 User name okay, need password."

    #define _332    "332 Need account for login."

    #define _425    "425 Use PORT or PASV first."

    #define _500    "500 Unknown command."

    #define _501    "501 Syntax error in parameters or arguments."

    #define _530    "530 Login with USER and PASS first."

    #define _550    "550 Failed to change directory."


#endif /* !ERROR_MESSAGES_H_ */
