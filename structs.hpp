#ifndef STRUCTS_HPP
#define STRUCTS_HPP


#define MAX_MSG_LEN 512
#define MAX_STR_LEN 1024
#define MAX_BUFF_LEN 100 


//    this struct holds data from the client until a whole command is received 
//    then I will transfer the data to command class 

typedef struct s_msg
{
    char    *msg;
    int     counter;
    bool    nick_cmd;
    bool    user_cmd;
}   t_msg;

t_msg recv_msg(char *buf, t_msg msg, Client *clients, int client_socket, char *client_hostname, char *server_hostname);

#endif