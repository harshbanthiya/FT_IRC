/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 10:16:39 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/06 13:40:23 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"

// ================ ESSENTIAL ===================
// ================ SERVER COMMANDS =============

Server::Server(std::string _port, std::string passwd) : port(_port), passwrd(passwd), handler(*this)
{

	int 	yes = 1;
	int 	ret;
	
	struct addrinfo hints, *ai;

	std::time_t result = std::time(nullptr);
	createdTime = std::asctime(std::localtime(&result));
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((ret = getaddrinfo(nullptr, port.c_str(), &hints, &ai)))
		throw std::runtime_error(gai_strerror(ret));
	if ((sock_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0)
		throw Server::SocketFailException();
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw Server::SetsockoptFailException();
	if ((bind(sock_fd, ai->ai_addr, ai->ai_addrlen) < 0))
	{
		close(sock_fd);
		freeaddrinfo(ai);
		throw Server::BindFailException();
	}
	freeaddrinfo(ai);
	load_motd(MOTD_PATH);
}

Server::~Server()
{
	close(sock_fd);
	for(u_int i = 0; i < list_of_all_clients.size(); i++)
		disconnect_client(i);
}

void Server::execute()
{
	if ((listen(sock_fd, BACKLOG_CONNECTIONS)) < 0)
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
					accept_client();
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
						curr.buffer() += buf;
						if (curr.buffer().find(END_DELIM) != std::string::npos)
							exec_command(curr);
					}
				}
			}
		}
	}
}

void 	Server::load_motd(const char *file)
{
	std::fstream	my_file;
	
	my_file.open(file, std::ios::in);
	if (!my_file)
		return ;
	my_file >> std::noskipws;

	std::string	line;
	char 		ch;

	do{
		my_file >> ch;
		if (ch == '\n')
		{
			motd.push_back(line);
			line.clear();
		}
		else 
			line += ch;
	}while (!my_file.eof());
	my_file.close();
}

void 	Server::accept_client()
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
		inet_ntop(AF_INET, &(((struct sockaddr_in *)casted_addr)->sin_addr), remoteIP, INET_ADDRSTRLEN);
	else 
		inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)casted_addr)->sin6_addr), remoteIP, INET6_ADDRSTRLEN);
	list_of_all_clients.push_back(new Client(newfd, remoteIP));
}

void 		Server::disconnect_client(int index)
{
	close(pfds[index].fd);
	pfds.erase(pfds.begin() + index);
	delete(list_of_all_clients[index - 1]);
	list_of_all_clients.erase(list_of_all_clients.begin() + index - 1);
}

void 		Server::disconnect_client(std::string nick)
{
	for (u_int i = 0; i < list_of_all_clients.size(); i++)
	{
		if (*this->list_of_all_clients[i] == nick)
			return (disconnect_client(i + 1));
	}
}

void Server::add_fd(int new_fd)
{
	struct pollfd tmp;

	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	tmp.fd = new_fd;
	tmp.events = POLLIN;
	pfds.push_back(tmp);
}

void Server::exec_command(Client &exec)
{
	std::string &buffer = exec.buffer();
	int pos = buffer.find(END_DELIM);
	do
	{
		handler.handle(buffer.substr(0, pos), exec);
		buffer.erase(0, pos + 2);
		pos = buffer.find(END_DELIM);
	}while (pos != -1);
}

// ================ CHECK FUNCTIONS ===================

bool	Server::check_pass(std::string &pass){return (pass == this->passwrd);}

bool Server::user_exists(std::string name)
{
	for (size_t i = 0; i < list_of_all_clients.size(); i++)
		if (*list_of_all_clients[i] == name)
			return true;
	return false;
}

bool 	Server::check_channel(std::string target) const
{
	if (list_of_all_channels.find(target) != list_of_all_channels.end())
		return true;
	else
		return false;
}

// ================ ALL TYPES OF SEND MSGS ===================

void 	Server::send_msg(std::string &msg, Client const &target) const
{
	if (send(target.get_socket(), msg.c_str(), msg.length(), 0) < 0)
	 	perror("send()");
}

int 	Server::send_msg(std::string &msg, std::string target) const
{
	u_int 	i = 0;

	while (i < list_of_all_clients.size())
	{
		if (*list_of_all_clients[i] == target)
		{
				send_msg(msg, *list_of_all_clients[i]);
				break ;
		}
		i++;
	}
	if (i == list_of_all_clients.size())
		return (ERR_NOSUCHNICK);
	return (0);
}

int		Server::send_msg(std::string &msg, std::string target, Client const &owner)
{
	if (check_channel(target))
	{
		Channel& tmp_chan = get_channel(target); 
		if (tmp_chan.can_send_msg(owner)) 
			tmp_chan.send_to_all(msg);
		else 
			return (0);
	}
	else 
		return (ERR_NOSUCHNICK);
	return (0);
}

// ================ OTHER CHANNEL COMMANDS ===================

void	Server::create_channel(std::string ch_name){list_of_all_channels[ch_name];}
void	Server::remove_channel(std::string name){list_of_all_channels.erase(name);}

bool 			Server::add_channel(Channel ch)
{
	if (list_of_all_channels.find(ch.get_name()) == list_of_all_channels.end())
	{
		list_of_all_channels.insert(std::pair<std::string, Channel>(ch.get_name(), ch));
		return (true);
	}
	return (false);
}

// ================ GETTERS ===================

std::vector<Client *> const &Server::get_all_clients(){ return (this->list_of_all_clients);}
std::string Server::getPasswrd() {return passwrd;}
Channel	 &Server::get_channel(std::string channelName){return (list_of_all_channels[channelName]);}
std::string Server::getcreatedTime() { return createdTime; }
CommandHandler Server::getHandler() const { return (handler); }
std::vector<std::string>const &Server::get_motd()const {return (motd);}

Client const		&Server::get_client(std::string nickname) const
{
	size_t 	i = 0;
	for (size_t i = 0; i < list_of_all_clients.size(); i++)
	{
		if (*list_of_all_clients[i] == nickname)
			return (*list_of_all_clients[i]);
	}
	return (*list_of_all_clients[i]);
}


// ================ EXCEPTIONS ===================

const char*	Server::SocketFailException::what() const throw(){return "Error:  Socket Failed\n";}
const char*	Server::SetsockoptFailException::what() const throw(){return "Error: Setsockopt() Failed\n";}
const char*	Server::BindFailException::what() const throw(){return "Error: Bind() Failed\n";}
const char*	Server::ListenFailException::what() const throw(){return "Error: listen() Failed\n";}
const char*	Server::FcntlFailException::what() const throw(){return "Error: Fcntl() Failed\n";}