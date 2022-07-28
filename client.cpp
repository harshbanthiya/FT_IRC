/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 16:18:40 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/07/27 16:56:36 by hbanthiy         ###   ########.fr       */
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
#include <netdb.h>


int main()
{
    int     client;
    int     portNum = 1500;

    bool    isExit = false;
    int     bufsize = 1024;

    char    buffer[bufsize];
    char    *ip = "127.0.0.1";

    struct sockaddr_in server_addr;

    // init socket 

    client = socket(AF_INET, SOCK_STREAM, 0);
      if (client < 0)
    {
        std::cout << "Error creating socket.. " << std::endl;
        exit(1);
    }

    std::cout << "Client Socket created.." << std::endl;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    // connecting socket server
    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        std::cout << "Connecting to server.." << std::endl;
    
    recv(client, buffer, bufsize, 0);
    std::cout << "Connection confirmed" << std::endl;
    std::cout << "Enter # to end the connection " << std::endl;

    do{
        std::cout << "Client: ";
        do{
            std::cin >> buffer;
            send(client, buffer, bufsize, 0);
            if (*buffer == '#')
            {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
        }while (*buffer != 42);
        std::cout << "Server: ";
        do{
            recv(client, buffer, bufsize, 0);
            std::cout << buffer << " ";
            if (*buffer == '#')
            {
                *buffer = '*';
                isExit = true;
            }
        }while (*buffer != 42);
        std::cout << std::endl;
    }while (!isExit);
    std::cout << "Connection terminated.. \n";
    std::cout << "Goodbye.. \n";

    close(client);
    return (0);
}