#include "Server.hpp"
#define MAX_CLIENTS 20
#define MAX_BUFF_SIZE 4096
#define END_DELIM "\r\n"

irc::Server::Server(unsigned int port, std::string passwd) : port(port), passwrd(passwd)
{
}

irc::Server::Server( const Server & src )
{
}


irc::Server::~Server()
{
	std::vector<Client *> clients = get_all_clients();
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		disconnect_client(*(*it));
}

void 	irc::Server::acceptClient()
{
	if (list_of_all_clients.size() == MAX_CLIENTS)
	{
		close(fd);
		return ;
	}
	struct 	sockaddr_in 	address;
	socklen_t 	csin_len 	= sizeof(address);
	int 	fd = accept(this->fd, (struct sockaddr *)&address, &csin_len);
	if (fd == -1)
		return ;
	list_of_all_clients[fd] = new Client(fd, address);
	pfds.push_back(pollfd());
	pfds.back().fd = fd;
	pfds.back().events = POLLIN;

	if (DEBUG)
		std::cout << "new user" << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << " (" << fd << ")" << std::endl; 
 }

// Init and Execute 
void 	irc::Server::init()
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

void irc::Server::execute()
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
				if ((*it).revents == POLLIN)
					this->list_of_all_clients[(*it).fd]->receive_from(this);
	}
	for (std::vector<irc::Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		if ((*it)->get_status() == DELETE)
			disconnect_client(*(*it));
	clients = get_all_clients();
	for (std::vector<irc::Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		(*it)->push();
	
}

std::vector<irc::Client *> irc::Server::get_all_clients()
{
	std::vector<irc::Client *> clients = std::vector<irc::Client *>();

	for (std::map<int, irc::Client *>::iterator it = this->list_of_all_clients.begin(); it != this->list_of_all_clients.end(); ++it)
			clients.push_back(it->second);
	return clients;
}
irc::Client *			  irc::Server::get_client(std::string &nickname)
{
	for (std::map<int, irc::Client *>::iterator it = this->list_of_all_clients.begin(); it != this->list_of_all_clients.end(); ++it)
	{
		if ((*it).second->get_nickname() == nickname)
			return ((*it).second);
	}
	return (NULL);
}

void 		irc::Server::disconnect_client(irc::Client &client)
{
	// Make logic here to go through each channel the user is a part of and remove it 
	for(std::vector<pollfd>::iterator it_p = pfds.begin(); it_p != pfds.end(); ++it_p)
		if((*it_p).fd == client.get_fd())
		{
			pfds.erase(it_p);
			break ;
		}
	list_of_all_clients.erase(client.get_fd());
	delete &client;
}

// Exceptions 
const char*	irc::Server::SocketFailException::what() const throw()
{
	return "Error:  Socket Failed\n";
}

const char*	irc::Server::SetsockoptFailException::what() const throw()
{
	return "Error: Setsockopt() Failed\n";
}

const char*	irc::Server::BindFailException::what() const throw()
{
	return "Error: Bind() Failed\n";
}

const char*	irc::Server::ListenFailException::what() const throw()
{
	return "Error: listen() Failed\n";
}
const char*	irc::Server::FcntlFailException::what() const throw()
{
	return "Error: Fcntl() Failed\n";
}