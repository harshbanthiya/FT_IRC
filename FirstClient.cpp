#include <netinet/in.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_N 9999



int main()
{
    int ret = 0;

    int C_socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET is for UDP/TCP connections 
                                                              // SOCK_STREAM is Connection oriented program, TCP 
                                                             // Finally the protocol we are using is TCP 
    if (C_socketFd < 0)
    {
        std::cout << "Socket didnt open \n";
        exit(1);
    }
    else 
        std::cout << "Socket opening successful. " << C_socketFd << '\n';

    struct sockaddr_in serv;

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_N); // Host to network short 
    serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of my own machine  if  you need to put another IP address 
                                       // you will write inet_addr("127.0.0.0"); 

    memset(&(serv.sin_zero), 0, 8); // Gotta initialise the array to zeroes 
    ret = connect(C_socketFd, (struct sockaddr *)&serv, sizeof(serv));
    if (ret < 0)
    {
        std::cout << "connection failed \n";
        exit(1);
    }
    char    sBuff[1024] = {0,};
    while (1)
    {
        printf("\n Type Message\n");
        fgets(sBuff, 1023, stdin);
        send(C_socketFd, sBuff, strlen(sBuff), 0);
    }
    
    
    /*
    else 
    {
        std::cout << "connection successful.\n";
        char buff[255] = {0,};
        recv(C_socketFd, buff, 255, 0);
        std::cout << "Press any key to see the text received from the server\n";
        getchar();
        std::cout << buff << '\n';
        std::cout << "Type in the message to send to the server" << std::endl;
        
        while (1)
        {
            fgets(buff, 256, stdin);
            send(C_socketFd, buff, 256, 0);
            std::cout << "Press a key to get a response from the server \n";
            getchar();
            recv(C_socketFd, buff, 256, 0);
            std::cout << buff << '\n' << "You can type your next message \n";

        }
    }
    */
}