#include "Server.hpp"
#include "Utils.hpp"
#define MAX_CLIENTS 20
#define MAX_BUFF_SIZE 4096
#define END_DELIM "\r\n"

irc::Server::Server(std::string _port, std::string passwd) : port(_port), last_ping(std::time(0)), passwrd(passwd)
{
	std::time_t result = std::time(nullptr);
	createdTime = std::asctime(std::localtime(&result));
}

irc::Server::~Server()
{
	close(sock_fd);
	for(u_int i = 0; i < list_of_all_clients.size(); i++)
		disconnect_client(i);
}

void 	irc::Server::acceptClient()
{

	struct sockaddr_storage clientaddr;
	socklen_t addrlen = sizeof(clientaddr);
	int 	newfd;

	newfd = accept(this->sock_fd, (struct sockaddr *)&clientaddr, &addrlen);
	if (newfd < 0)
		return(perror("accept()"));
	add_fd(newfd);
	char remoteIP[INET6_ADDRSTRLEN];
	struct sockaddr *casted_addr = (struct sockaddr *)&clientaddr;
	if (casted_addr->sa_family == AF_INET)
	{
		if (DEBUG)
			std::cout << "new user" << inet_ntop(AF_INET, &(((struct sockaddr_in *)casted_addr)->sin_addr), remoteIP, INET_ADDRSTRLEN) << ":" << "fd " << sock_fd << '\n';
		inet_ntop(AF_INET, &(((struct sockaddr_in *)casted_addr)->sin_addr), remoteIP, INET_ADDRSTRLEN);
	}
	else 
	{
		if (DEBUG)
			std::cout << "new user" << inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)casted_addr)->sin6_addr), remoteIP, INET6_ADDRSTRLEN) << ":" << "fd" << sock_fd << '\n';
		inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)casted_addr)->sin6_addr), remoteIP, INET6_ADDRSTRLEN);
	}
	list_of_all_clients.push_back(new Client(newfd, remoteIP));
 }

/*
void irc::Server::sendPing()
{
	time_t 	current =std::time(0);
	//int timeout = 3000;

	for (std::vector<irc::Client*>::iterator it = list_of_all_clients.begin(); it != list_of_all_clients.end(); ++it)
		if (current - (*it).second->getLastPing() >= 300)
		{
			(*it).second->set_status(DELETE);
		}
		else if ((*it).second->get_status() == ONLINE)
			(*it).second->write("PING" + (*it).second->get_nickname());
}
*/

// Init and Execute 
void 	irc::Server::init()
{
	int 	yes = 1;
	int 	ret;

	struct addrinfo hints, *ai;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((ret = getaddrinfo(nullptr, port.c_str(), &hints, &ai)))
		throw std::runtime_error(gai_strerror(ret));
	if ((sock_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0)
		throw Server::SocketFailException();
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		throw Server::SetsockoptFailException();
	
	if ((bind(sock_fd, ai->ai_addr, ai->ai_addrlen) < 0))
	{
		close(sock_fd);
		freeaddrinfo(ai);
		throw Server::BindFailException();
	}
	freeaddrinfo(ai);
}

void irc::Server::execute()
{
	if ((listen(sock_fd, 10)) < 0)
		throw Server::ListenFailException();

	add_fd(sock_fd);
	
	while (1)
	{
		if (poll(pfds.data(), pfds.size(), -1) < 0)
			throw std::runtime_error(strerror(errno));
		for (u_int i = 0; i < pfds.size(); i++)
		{
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == sock_fd)
					acceptClient();
				else 
				{
					char buf[512];
					memset(buf, 0, sizeof(buf));
					int nbytes = recv(pfds[i].fd, buf, sizeof(buf), 0);
					if (nbytes <= 0)
					{
						if (nbytes)
							perror("recv()");
						else 
							disconnect_client(i);

					}
					else 
					{
						Client &curr = *list_of_all_clients[i - 1];
						curr.buffer += buf;
						if (curr.buffer.find(END_DELIM) != std::string::npos)
							exec_command(curr);
					}
				}
			}
		}
	}
}

std::vector<irc::Client *> irc::Server::get_all_clients()
{
	return (this->list_of_all_clients);
}
irc::Client&			  irc::Server::get_client(std::string nickname)
{
	size_t 	i = 0;
	for (; i < list_of_all_clients.size(); i++)
	{
		if (list_of_all_clients[i] == nickname)
			return (*list_of_all_clients[i]);
	}
	return (*list_of_all_clients[i]);
}
std::string irc::Server::getPasswrd() {
	return passwrd;
}

std::string irc::Server::getcreatedTime() { return createdTime; }

void 		irc::Server::disconnect_client(std::string nick)
{
	for (u_int i = 0; i < list_of_all_clients.size(); i++)
	{
		if (list_of_all_clients[i] == nick)
			return (disconnect_client(i + 1));
	}
}


void 		irc::Server::disconnect_client(int index)
{
	close(pfds[index].fd);
	pfds.erase(pfds.begin() + index);
	delete(list_of_all_clients[index - 1]);
	list_of_all_clients.erase(list_of_all_clients.begin() + index - 1);
}

void irc::Server::add_fd(int new_fd)
{
	struct pollfd tmp;

	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	tmp.fd = new_fd;
	tmp.events - POLLIN;
	pfds.push_back(tmp);
}

void irc::Server::exec_command(Client &exec)
{
	std::string &buffer = exec.buffer;
	int pos = buffer.find(END_DELIM);
	do
	{
		handler.handle(buffer.substr(0, pos), exec);
		buffer.erase(0, pos + 2);
		pos = buffer.find(END_DELIM);
	}while (pos != -1)
	

}

bool irc::Server::user_exists(std::string name)
{
	size_t 	i = 0;
	for (; i < list_of_all_clients.size(); i++)
	{
		if (list_of_all_clients[i] == name)
			return true;
	}
	return false;
}

int 	irc::Server::send_msg(std::string &msg, Client &target)
{
	if (send(target.getSocket(), msg.c_str(), msg.length(), 0) < 0)
	 	perror("send()");
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