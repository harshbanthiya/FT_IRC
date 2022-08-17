#include "Server.hpp"
#define MAX_CLIENTS 20
#define MAX_BUFF_SIZE 4096
#define END_DELIM "\r\n"

Server::Server(unsigned int port, std::string passwd) : port{port}, passwrd{passwd}
{
}

Server::Server( const Server & src )
{
}


Server::~Server()
{
}

Server &				Server::operator=( Server const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

void 	Server::acceptClient()
{
	if (list_of_all_clients.size() == MAX_CLIENTS)
		close(fd);
	struct 	sockaddr_in6 	address;
	socklen_t 	csin_len 	= sizeof(address);
	int 	fd = accept(this->fd, (struct sockaddr *)&address, &csin_len);
	if (fd == -1)
		return ;
	list_of_all_clients[fd] = new Client(fd, address);
	pfds.push_back(pollfd());
	pfds.back().fd = fd;
	pfds.back().events = POLLIN;
}

// Init and Execute 
void 	Server::init()
{
	int 	yes = 1;

	if ((fd = socket(AF_INET6, SOCK_STREAM, 0)) == -1)
		throw Server::SocketFailException();
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw Server::SetsockoptFailException();
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		throw Server::FcntlFailException();

	addr_info.sin6_family = AF_INET6;
	addr_info.sin6_addr = in6addr_any;
	addr_info.sin6_port = htons(port);
	
	if ((bind(fd, (struct sockaddr*)&this->addr_info, sizeof(this->addr_info)) == -1))
		throw Server::BindFailException();
	if (listen(fd, addr_info.sin6_port) == -1)
		throw Server::ListenFailException();

	pfds.push_back(pollfd());
	pfds.back().fd = fd;
	pfds.back().events = POLLIN;
}

void Server::execute()
{
	std::vector<Client *> clients = get_all_clients();
	if (poll(&pfds[0], pfds.size(), -1) == -1)
		return ;
	else 
	{
		if (pfds[0].revents == POLLIN)
			acceptClient();
		else 
			for (std::vector<pollfd>::iterator it = pfds.begin(); it != pfds.end(); ++it)
			{	
				if((*it).revents == POLLIN)
				{
					char 	buffer[MAX_BUFF_SIZE + 1];
					ssize_t size;
					Client 	*cl;

					cl = list_of_all_clients[(*it).fd];
					size = recv((*it).fd, buffer, MAX_BUFF_SIZE, MSG_DONTWAIT);
					if (size <= 0)
					{
						remove_client((*it).fd);
						continue ;
					}
					buffer[size] = '\0';
					cl->append_buffer(0, std::string(buffer));
					if (!cl->get_registered())
					{
						//login
					}
				}
			{
				
	}
}


/*	
	std::string delimiter(END_DELIM);
	size_t 		position;
	while ((position = buffer.find(delimiter)) != std::string::npos)
	{
		std::string	message = buffer.substr(0, position);
		buffer.erase(0, position + delimiter.length());
		if (!message.length())
			continue ;
				
	}
}
*/
// Getters

std::vector<Client *> Server::get_all_clients()
{
	std::vector<Client *> clients = std::vector<Client *>();

	for (std::map<int, Client *>::iterator it = this->list_of_all_clients.begin(); it != this->list_of_all_clients.end(); ++it)
			clients.push_back(it->second);
	return clients;
}
Client *			  Server::get_client(std::string &nickname)
{
	for (std::map<int, Client *>::iterator it = this->list_of_all_clients.begin(); it != this->list_of_all_clients.end(); ++it)
	{
		if ((*it).second->get_nickname() == nickname)
			return ((*it).second);
	}
	return (NULL);
}

// Exceptions 
const char*	Server::SocketFailException::what() const throw()
{
	return "Error:  Socket Failed\n";
}

const char*	Server::SetsockoptFailException::what() const throw()
{
	return "Error: Setsockopt() Failed\n";
}

const char*	Server::BindFailException::what() const throw()
{
	return "Error: Bind() Failed\n";
}

const char*	Server::ListenFailException::what() const throw()
{
	return "Error: listen() Failed\n";
}
const char*	Server::FcntlFailException::what() const throw()
{
	return "Error: Fcntl() Failed\n";
}