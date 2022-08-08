#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>

#define PORT_N  5000 

int main()
{

    // SERVER
    struct sockaddr_in serv = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT_N)
    };

    int option = 1;
    int saddrSize = sizeof(serv);
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
    if (socketServer == -1)
    {
        std::cerr << "Not able to create socket \n";
        return (-1);
    }

    // CLIENT

    struct sockaddr_in client_addr;
    socklen_t   caddrSize =  sizeof(client_addr);
    int     socketClient;

    // Bind socket to ip+port
    bind(socketServer, (struct sockaddr*)&serv, sizeof(serv));

    // Listening 
    listen(socketServer, SOMAXCONN);
    // Print
    std::stringstream ss;
    ss << PORT_N;
    std::cout << "[Server] listening on port " << ss.str() << std::endl;



    char buff[4096];
    int sizeInBytesOfReceivedData;

    // while waiting for client

    while (true)
    {
        // Accept connection from clients;
        socketClient = accept(socketServer, (struct sockaddr*)&client_addr, (socklen_t*)&caddrSize);
        std::cout << "[Server] Client connection successful." << std::endl;

        // Try to find out who is the client 
        char hostClient[NI_MAXHOST];
        char portClient[NI_MAXSERV];
        memset(hostClient, 0, NI_MAXHOST);
        memset(portClient, 0, NI_MAXSERV);
        if (getnameinfo((sockaddr*)&client_addr, sizeof(client_addr), hostClient, NI_MAXHOST, portClient, NI_MAXSERV, 0) == 0)
        {
            std::cout << "-->" << hostClient << " connected to the port " << portClient << std::endl;
        }
        else 
        {
            inet_ntop(AF_INET, &client_addr.sin_addr, hostClient, NI_MAXHOST);
            std::cout << "--> " << hostClient << " Connected to the port " << ntohs(client_addr.sin_port) << std::endl;
        }
        
        // Receive our data
        sizeInBytesOfReceivedData = recv(socketClient, buff, 4096, 0);
        if (sizeInBytesOfReceivedData == -1)
        {
            std::cerr << "Error receiving message. Quiting";
            break;
        }
        else if (sizeInBytesOfReceivedData == 0)
        {   
            std::cout << "Client Disconnected" << std::endl;
            break ;
        }
        send(socketClient, buff, sizeInBytesOfReceivedData + 1, 0);
        std::cout << std::string(buff, 0, sizeInBytesOfReceivedData) << std::endl;
        close(socketClient);
    }

    return (0);

}