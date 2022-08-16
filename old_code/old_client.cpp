#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#define PORT_N "5000"
#define MAXDATASIZE 100 

// get sockaddr, Ipv4 or Ipv6
void    *get_in_addr(struct sockaddr *s)
{
    if (s->sa_family == AF_INET)
        return &(((struct sockaddr_in *)s)->sin_addr);
        
    return &(((struct sockaddr_in6 *)s)->sin6_addr);   
}

int main(int argc, char *argv[])
{
    int                 sockfd, numbytes;
    char                buff[MAXDATASIZE];
    struct  addrinfo    hints, *servinfo, *p;
    int                 ret;
    char                s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        std::cerr << "Usage: ./client  hostname";
        exit(1);
    }

    // Setting up the struct 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((ret = getaddrinfo(argv[1], PORT_N, &hints, &servinfo)) != 0)
    {
        std::cerr << "Client setup failed " << gai_strerror(ret) << " quiting!\n";
        return (1); 
    }

    // Loop through and connect to the first one
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "Client : Socket Failed \n";
            continue ;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            std::cerr << "Cient : Connection failed \n";
            continue ;
        }
        break ;
    }

    if (p == NULL)
    {
        std::cerr << "Client: has failed to connect! Quiting!\n";
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
    std::cout << "Client : connecting to " << s << std::endl;
    freeaddrinfo(servinfo);

    if ((numbytes = recv(sockfd, buff, MAXDATASIZE - 1, 0)) == -1)
    {
        std::cerr << "client : recv error \n";
        exit(1);
    }

    buff[numbytes] = '\0';
    std::cout << buff << '\n';
    close(sockfd);
    return (0);

}