#include "Server.hpp"

Server::Server(unsigned int port, std::string passwd) : port{port}, passwrd{passwd}
{
	// Maybe make an initialiser function 
	p_fd.events = 0;
	p_fd.revents = 0;
	p_fd.fd = 0;
	int yes = 1; 
	total_client_count = 0;
	addr_info.sin6_family = AF_INET6;
	addr_info.sin6_addr = in6addr_any;
	addr_info.sin6_port = htons(port);
	
	p_fd.fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (p_fd.fd == -1)
		throw Server::SocketFailException();
	if (setsockopt(p_fd.fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw Server::SetsockoptFailException();

	if ((bind(p_fd.fd, (struct sockaddr*)&this->addr_info, sizeof(this->addr_info)) == -1))
		throw Server::BindFailException();
	if (listen(p_fd.fd, 5) == -1)
		throw Server::ListenFailException();
	p_fd.events = POLLIN;
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