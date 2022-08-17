/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:08:30 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/17 14:48:20 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#define MAX_SIZE 4096
#define END_DELIM "\r\n"

Client::Client(int _fd, sockaddr_in6 addr_) :command_function(), 
											 fd(_fd), 
											 buffer(),
											 commands(),
											 waitingtoSend(),
											 status(PASSWORD),
											 client_addr(), 
											 nickname(""), 
											 username(""), 
											 realname(""), 
											 hostname("")
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
	client_addr = inet_ntoa((sockaddr_in)addr_.sin6_addr);
	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&address, sizeof(address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
	{
		std::cerr << "Error: getnameinfo" << std::endl;
		exit(1);
	}
	else 
		this->hostname = hostname;
	// remember to implement IP address function. 
	command_function["PASS"]; //= // PASS FUNCTION POINTER ;
	command_function["NICK"]; //= ; // NICK FUNCTION POINTER

	client_ip_addr = "";
	nickname_set = false;
	user_set = false;
	registered = false;
} 

Client::Client( const Client & rhs){*this = rhs;}
Client::~Client(){close(fd);}

Client&		Client::operator=( Client const & rhs )
{
	if ( this == &rhs )
		return (*this);
	this->nickname = rhs.nickname;
	this->username = rhs.username;
	this->realname = rhs.realname;
	this->hostname = rhs.hostname;
	this->fd =  rhs.fd;
	return *this;
}

// Getters
std::string	 		Client::get_nickname(void) const {return nickname;}
std::string	 		Client::get_username(void) const {return username;}
std::string	 		Client::get_realname(void) const {return realname;}
std::string	 		Client::get_hostname(void) const {return hostname;}
std::string	 		Client::get_client_ip_addr(void) const {return client_ip_addr;}
bool 				Client::get_nick_bool(void) {return nickname_set;}
bool 				Client::get_user_bool(void) {return user_set;}
bool 				Client::get_registered(void) {return registered;}
std::string&		Client::get_buffer(int std_n)
{
	if (std_n == 0)
		return buffer[0];
	if (std_n == 1)
		return buffer[1];
	return buffer[0];
}

// Setters 
void 				Client::set_nickname(std::string nick_n){nickname = nick_n;}
void 				Client::set_username(std::string user_n){username = user_n;}
void 				Client::set_realname(std::string real_n){realname = real_n;}
void 				Client::set_hostname(std::string host_n){hostname = host_n;}
void 				Client::set_client_ip_addr(std::string ip_addr){client_ip_addr = ip_addr;}
void 				Client::set_nick_bool(bool new_val){nickname_set = new_val;}
void 				Client::set_user_bool(bool new_val){user_set = new_val;}
void 				Client::set_registered(bool new_val){registered = new_val;}

void	Client::append_buffer(int std_n, const std::string& content )
{
	if (std_n == 0)
		buffer[0].append(content);
	if (std_n == 1)
		buffer[1].append(content);
}

void	Client::clear_buffer(int std_n)
{
	if (std_n == 0)
		buffer[0].clear();
	if (std_n == 1)
		buffer[1].clear();
}

std::string Client::getprefix()
{
	if (status == PASSWORD || status == REGISTER)
		return std::string("");

	std::string prefix = nickname;
	if (get_hostname().length())
	{
		if (username.length())
			prefix += "!" + username;
		prefix += "@" + get_hostname();
	}
	return prefix;
}

void Client::receive_from(Server *server)
{
	char buffer[MAX_SIZE];
	ssize_t size;

	size = recv(fd, &buffer, MAX_SIZE, 0);
	if (size == -1)
		return ;
	if (size == 0)
	{
		status = DELETE;
		return ;
	}
	buffer[size] = '\0';
	this->append_buffer(0, std::string(buffer));

	std::string delimiter(END_DELIM);
	size_t 		position;
	while ((position = this->buffer[0].find(delimiter)) != std::string::npos)
	{
		std::string message = this->buffer[0].substr(0, position);
		this->buffer[0].erase(0, position + delimiter.length());
		if (!message.length())
			continue;
		commands.push_back(new Command(this, server, message));
	}
	dispatch();
}

void Client::write(std::string message){waitingtoSend.push_back(message);}

void Client::push()
{
	if (!buffer[1].size())
		return ;
	std::string buffer;
	for (std::vector<std::string>::iterator it = waitingtoSend.begin(); it != waitingtoSend.end(); ++it)
		buffer += *it + END_DELIM;
	waitingtoSend.clear();
	if (buffer.length())
		if (send(fd, buffer.c_str(), buffer.length(), 0) == -1)
		{
			std::cerr << "error: send()" << std::endl;
			exit(1);
		}
}
void Client::sendTo(Client &toclient, std::string message) { toclient.write(":" + this->getprefix() + " " + message); }