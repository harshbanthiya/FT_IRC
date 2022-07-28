/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 16:03:14 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/07/27 16:58:47 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int     client, server;
    int     portNum = 1500;

    bool    isExit = false;
    int     bufsize = 1024;

    char    buffer[bufsize];
    
    struct sockaddr_in server_addr;
    socklen_t size;

    // init sockets

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0)
    {
        std::cout << "Error estabishing connection" << std::endl;
        exit(1);
    }

    std::cout << "Server Socket Connection created.." << std::endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    // binding socket 

    if (bind(client, (struct  sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        std::cout << "Error binding socket..." << std::endl;
        return (-1);
    }

    size = sizeof(server_addr);
    std::cout << "Looking for clients.." << std::endl;

    // Listening socket 
    listen(client, 1);

    // Accept Client 
    server = accept(client ,(struct sockaddr *)&server_addr, &size);

    if (server < 0)
    {
        std::cout << "Error on accepting .. " << std::endl;
      //  exit(1);
    }


    while (server > 0)
    {
        strcpy(buffer, "Server Connected...\n");
        send(server, buffer, bufsize, 0);

        std::cout << "Connected with client..." << std::endl;
        std::cout << "Enter # to end the connection " << std::endl;
        std::cout << "Client :";
        do{
            recv(server, buffer, bufsize, 0);
            std::cout << buffer << " ";
            if (*buffer == '#')
            {
                *buffer = '*';
                isExit = true;
            }
        }while(*buffer != '*');

        do{
            std::cout << "\n Server: " ;
            do{
                std::cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#')
                {
                    send(server,buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            }while (*buffer != '*');


            std::cout << "Client: ";
            do{
                recv(server, buffer, bufsize, 0);
                std::cout << buffer << " ";
                if (*buffer == '#')
                {
                    *buffer = '*';
                    isExit = true;
                }
            }while (*buffer != '*');
        

        }while(!isExit);
        std::cout << "Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        std::cout << "Goodbye" << std::endl;
        isExit = false;
        exit(1);
    }
    close(client);
    return (0);
}
