#include "structs.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include <string.h>


Command parse_msg(char *msg_buffer)
{
    char    *token;
    char    *rest = msg_buffer;

    Command  parsed_cmd;
    parsed_cmd.set_num_params(0);
    int     counter = 0;

    // Flag to indicate that a param takes up rest of the message 
    bool    param_is_rest = false;
    while ((token = strtok_r(rest, " \t\r\n", &rest)))
    {
        if (counter == 0)
        {
            // token is command keyword 
            parsed_cmd.set_command(static_cast<std::string>((const char *)token)); // check this conversion later   
        }
        else if (counter < MAX_PARAMS)
        {
            // Token is a param 

            if (!param_is_rest)
            {
                // Param does not take up rest of the message 
                if (token[0] == ':')
                    param_is_rest = true;
                token = strtok(token, ":");
                parsed_cmd.set_param(counter - 1, static_cast<std::string>((const char *)token)); 
            }
            else 
            {
                // Param takes up rest of the message 
                // Concatatenate it 
                std::string param_so_far = parsed_cmd.get_param(counter - 1);
                std::string concat_param = param_so_far + " " + static_cast<std::string>((const char *)token);
            }
        }
        if (!param_is_rest)
        {
            // counter only increments when param does not take up the rest of the message 
            counter++;
        }
    }
    parsed_cmd.set_num_params(counter);
    return (parsed_cmd);
}

void    exec_msg(int client_socket, Client *clients, char *client_hostname, char *server_hostname, Command msg)
{
     // Execute parsed message 

    char *reply_code = new char (3);
    Client *client;
    for(int i = 0; i < 5 ; i++)
    {
        client = clients[i].get_client_by_hostname(static_cast<std::string>((const char *)client_hostname));
        if (client != NULL)
            break ;
    }
    if (msg.get_command().compare("NICK") == 0)
    {
        std::string nick = msg.get_param(0);
        if (client == NULL)
        {
            Client new_client(nickname, NULL, NULL, static_cast<std::string>((const char *)client_hostname), NULL);
        }
    }
}


/*
    Receives and processes incoming messages 
    buf : buffer for incoming message
    msg: holds data until a whole command is formed
    clients: collection of clients 
    Returns the type t_msg so it can be updated in each loop call
*/

t_msg recv_msg(char *buf, t_msg msg, Client *clients, int client_socket, char *client_hostname, char *server_hostname)
{
    char    c;

    for(int i = 0; i < strlen(buf); i++)
    {
        c = buf[i];
        // truncate message if it contains more than 512 characters
        if (msg.counter == MAX_MSG_LEN)
        {
            char copy_msg[MAX_MSG_LEN];
            strcpy(copy_msg, msg.msg);
            // Process it 
            exec_msg(client_socket, clients, client_hostname, server_hostname, parse_msg(copy_msg));
            // wipe msg buffer and renew counter after current command is sent to be processed 
            char *new_msg = new char[(sizeof(char) * MAX_MSG_LEN)];
            msg.msg = new_msg;
            msg.counter = 0;
            Command cmd = parse_msg(copy_msg);
            // check if command is nick or user if yes turn boolean field to true 
            if (msg.nick_cmd == false)
            {
                if ((cmd.get_command()).compare("NICK") == 0)
                    msg.nick_cmd = true;
            }
            if (msg.user_cmd == false)
            {
                if ((cmd.get_command()).compare("USER") == 0)
                    msg.user_cmd = true;
            }
            return (msg);
        }
        else 
        {
            // When is not overflowing 
            msg.msg[msg.counter] = c;
            // if end of command 
            if (c == '\n')
            {
                if (msg.msg[msg.counter - 1] == '\r')
                {
                    /* copy the message to a new char array to process it */
                    char copy_msg[strlen(msg.msg)];
                    strcpy(copy_msg, msg.msg);
                    exec_msg(client_socket, clients, client_hostname, server_hostname, parse_msg(copy_msg));
                    char *new_msg = new char[(sizeof(char) * MAX_MSG_LEN)];
                    msg.msg = new_msg;
                    msg.counter = 0;
                    Command cmd = parse_msg(copy_msg);
                    if (msg.nick_cmd == false)
                    {
                        if ((cmd.get_command()).compare("NICK") == 0)
                            msg.nick_cmd = true;
                    }
                    if (msg.user_cmd == false)
                    {
                        if ((cmd.get_command()).compare("USER") == 0)
                            msg.user_cmd = true;
                    }

                }
                else 
                    msg.counter++;
            }
            msg.counter++;
        }
    }
    return msg;    
}