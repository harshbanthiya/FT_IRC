#include <netinet/in.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#define PORT_N 9999
/*
    struct sockaddr_in {
        short       sin_family;  family
        u_short     sin_port;    port 
        struct      in_addr sin_addr;  machine address / IP address
        char        sin_zero[8];    special array needs to be initialised to zero 
    }
*/

fd_set fr, fw, fe; // fd set is a struct with two things , fd_count and socketFd array
                   // socket descriptors for fr - reading, fw - writing and fe - exceptions 

int     socketFd;
int     nArrclient[5] = {0};
int     maxFd = 0;

void HandleNewConnection() 

{

	//nNewClient will be a new file descriptor

	//and now the client communication will take place 

	//using this file descriptor/socket only

	int nNewClient = accept(socketFd, NULL, NULL );

	//If you accept the value in second parameter, then it

	//will be 

	if (nNewClient < 0)

	{

		std::cout << std::endl << "Not able to get a new client socket";

	}

	else

	{

		int nIndex;

		for (nIndex = 0; nIndex < 5; nIndex++)

		{

			if (nArrclient[nIndex] == 0)

			{

				nArrclient[nIndex] = nNewClient;

				if (nNewClient > maxFd)

				{

					maxFd = nNewClient + 1;

				}

				break;

			}

		}



		if (nIndex == 5)

		{

			std::cout << std::endl << "Server busy. Cannot accept anymore connections";

		}

	}

}



void HandleDataFromClient()

{

	for (int nIndex = 0; nIndex < 5; nIndex++)

	{

		if (nArrclient[nIndex] > 0)

		{

			if (FD_ISSET(nArrclient[nIndex], &fr))

			{ 

				//Read the data from client

				char sBuff[255] = { 0 };

				int nRet = recv(nArrclient[nIndex], sBuff, 255, 0);

				if (nRet < 0)

				{

					//This happens when client closes connection abruptly

					std::cout << std::endl << "Error at client socket";

					close(nArrclient[nIndex]);

					nArrclient[nIndex] = 0;

				}

				else

				{

					std::cout << std::endl << "Received data from:"

						<< nArrclient[nIndex]

						<< "[Message:" << sBuff << "]";

					break;

				}

			}

		}

	}

}



void    processNewMessage(int cl_sock)
{
    std::cout << "Processing new message from client socket " << cl_sock << '\n';
    char buff[256 + 1] = {0, };
    int  ret = recv(cl_sock, buff, 256, 0);
    if (ret < 0)
    {
        std::cout << "Something went wrong, closing connection for client  " << cl_sock << '\n';
        close(cl_sock);
        for (int i = 0; i < 5; i++)
        {
            if (nArrclient[i] == cl_sock)
            {
                nArrclient[i] = 0;
                break ;
            }
        }
    }
    else 
    {
        std::cout << "The message received from client - " << buff << '\n';
        send(cl_sock, "Processed your request", 23, 0);
        std::cout << "**********************";

    }
}

void    processNewRequest()
{
    if (FD_ISSET(socketFd, &fr))
    {
        unsigned int len = sizeof(struct sockaddr);
        int clientSocket = accept(socketFd, NULL, &len);
        if (clientSocket > 0)
        {
            int     index;
            // Put it into client fd_set
            for (index = 0; index < 5; index++)
            {
                if (nArrclient[index] == 0)
                {
                    nArrclient[index] = clientSocket;
                    send(clientSocket, "Connection request with server successful\n", 43, 0);
                    break ;
                }
            }
            if (index == 5)
                std::cout << "No space for a new client connection  \n";
        }
        
    }
    else 
    {
        for (int i = 0; i < 5; i++)
        {
            if (FD_ISSET(nArrclient[i], &fr))
            {
                // Got a new message from the client 
                // Just receive said message 
                // Just queue that message to be fulfilled later 
                processNewMessage(nArrclient[i]);
            }
        }
    }
}


int main()
{

    int     ret = 0;

    // Initialise socket 
    socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET is for UDP/TCP connections 
                                                              // SOCK_STREAM is Connection oriented program, TCP 
                                                             // Finally the protocol we are using is TCP 
    if (socketFd < 0)
    {
        std::cout << "Socket didnt open \n";
        exit(1);
    }
    else 
        std::cout << "Socket opening successful. " << socketFd << '\n';
    
    
    // initialise the environment for sockaddr 
    
    struct sockaddr_in serv;

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT_N); // Host to network short 
    serv.sin_addr.s_addr = INADDR_ANY; // IP address of my own machine  if  you need to put another IP address 
                                       // you will write inet_addr("127.0.0.0"); 

    memset(&(serv.sin_zero), 0, 8); // Gotta initialise the array to zeroes 
    /*
    // int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
    int n = 0;
    int optlen = sizeof(n);
    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (const void *) &n, optlen);
    if (!ret)
        std::cout << "The set sock option is successful.\n";
    else 
    {
        std::cout << "The set sock option failed \n";
        exit(1);
    }
*/
    // Bind the socket to the local port 

    ret = bind(socketFd, (struct sockaddr*)&serv, sizeof(struct sockaddr));
    if (ret < 0)
    {
        std::cout << "Socket binding is failed to bind to local port \n";
        exit(1);
    }
    else 
        std::cout << "Socket binding successful to local port \n";
    

    // listen to the request from client (queue the request)

    ret = listen(socketFd, 5); //second argument here is the backlog, more than 5 clients needs to wait in the wait queue 
    if (ret < 0)
    {
        std::cout << "Failed to start listening to local port \n";
        exit(1);
    }
    else 
        std::cout << "Stared listening to local to port \n";

    // Keep waiting for new requests and proceed as per the request 
    maxFd = socketFd + 1;
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    
    while (1)
    {

        // Initialise all socket fd to zero
        FD_ZERO(&fr);
        
        // set socketfd to read and exception not write;
        FD_SET(socketFd, &fr); 
      
    
        for (int i = 0; i < 5; i++)
        {
            if(nArrclient[i] > 0)
                FD_SET(nArrclient[i], &fr);
        }
    
        ret = select(maxFd, &fr, NULL, NULL, &tv);
        if (ret < 0)
        {
            std::cout << "Failed \n";
            exit (1);
        }
        else if (ret == 0)
        {
            std::cout << "no clients\n";
        }
        else 
        {
            if (FD_ISSET(socketFd, &fr))
                HandleNewConnection();
            else
                HandleDataFromClient();
        }
    
    }
    return (0);
}