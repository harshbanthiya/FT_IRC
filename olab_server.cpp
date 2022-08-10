#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
if (setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR , &option, sizeof(option)))
int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
		printf("here");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); // htons is host-to-network short
                                    // This means it works on 16-bit short integers. i.e. 2 bytes.
                                    // This function swaps the endianness of a short.
  
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

	while (true)
	{
		if ((new_socket
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		valread = read(new_socket, buffer, 1024);
		printf("%s\n", buffer);
		send(new_socket, hello, strlen(hello), 0);
		printf("Hello message sent\n");
		
	// closing the connected socket
		close(new_socket);
	}
	

  // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}


//  SO_REUSEADDR
//               Indicates that the rules used in validating addresses
//               supplied in a bind(2) call should allow reuse of local
//               addresses.  For AF_INET sockets this means that a socket
//               may bind, except when there is an active listening socket
//               bound to the address.  When the listening socket is bound
//               to INADDR_ANY with a specific port then it is not possible
//               to bind to this port for any local address.  Argument is
//               an integer boolean flag.

//        SO_REUSEPORT (since Linux 3.9)
//               Permits multiple AF_INET or AF_INET6 sockets to be bound
//               to an identical socket address.  This option must be set
//               on each socket (including the first socket) prior to
//               calling bind(2) on the socket.  To prevent port hijacking,
//               all of the processes binding to the same address must have
//               the same effective UID.  This option can be employed with
//               both TCP and UDP sockets.

//               For TCP sockets, this option allows accept(2) load
//               distribution in a multi-threaded server to be improved by
//               using a distinct listener socket for each thread.  This
//               provides improved load distribution as compared to
//               traditional techniques such using a single accept(2)ing
//               thread that distributes connections, or having multiple
//               threads that compete to accept(2) from the same socket.

//               For UDP sockets, the use of this option can provide better
//               distribution of incoming datagrams to multiple processes
//               (or threads) as compared to the traditional technique of
//               having multiple processes compete to receive datagrams on
//               the same socket.