#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Client.hpp"
#include "Command.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./server portnumber password" << '\n';
        return (1);
    }

    int                         serversocket;
    int                         clientsocket;
    char                        *passwd = argv[2];
    char                        *port = argv[1];
    struct addrinfo             hints, *res, *p;
    struct sockaddr_storage     *client_addr;
    socklen_t sin_size = sizeof(struct sockaddr_storage);
    int yes = 1;
    int numbytes;
    char buf[100];
    char msg[512];
    int ret;
    t_msg msg_recv = {"", 0, false, false};
    msg_recv.msg = msg;

    std::cout << "Port Number: " << port << " Password entered: " << passwd << '\n';
    //char *msg = ":bar.example.com 001 user1 :Welcome to the Internet Relay Network user1!user1@foo.example.com\r\n";

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (ret = (getaddrinfo(NULL, port, &hints, &res)) != 0)
    {
        std::cout << "Select Server: " << gai_strerror(ret) << '\n';
        exit(1);
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        if ((serversocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cout << "Socket Failed\n";
            continue ;
        }
        if (setsockopt(serversocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            std::cout << "set sock opt() failed \n";
            continue ;
        }
        if (bind(serversocket, p->ai_addr, p->ai_addrlen) == -1)
        {
            std::cout << "bind() failed\n";
            close(serversocket);
            continue ;
        }
        if (listen(serversocket, 5) == -1)
        {
            std::cout << "Listen() failed\n";
            close(serversocket);
        }
        break ;
    }
    freeaddrinfo(res);
    if (p == NULL)
    {
        std::cout << "Couldn't find a socket to bind to \n";
        exit(-1);
    }
    // Array of Clients 
   // Client clients[5];
    while (1)
    {
        clientsocket = accept(serversocket, (struct sockaddr *) &client_addr, &sin_size);
        // Another loop to listen receive until client enters a NICK and USER 
        while (!(msg_recv.nick_cmd && msg_recv.user_cmd))
        {
            if ((numbytes == recv(clientsocket, buf, sizeof (buf), 0)) == -1)
            {
                std::cout << "recv() failed \n";
                exit(1);
            }
            buf[numbytes] = '\0';
            char  server_hostname[MAX_STR_LEN];
            // Get name of host server
            gethostname(server_hostname, sizeof(server_hostname));
            char hostname[MAX_STR_LEN];
            char port[MAX_STR_LEN];
            // Get client host name
            int result = getnameinfo((struct sockaddr *)&client_addr, sin_size, hostname, sizeof(hostname), port, sizeof(port), 0);
            // Send all this data to parsing function to process.
           // msg_recv = recv_msg(buf, msg_recv, &clients, clientsocket, hostname, server_hostname);
        }
    }
    close(serversocket);
}