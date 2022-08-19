/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:08:30 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/19 13:56:18 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <fcntl.h>
#include "Utils.hpp"

#define MAX_SIZE 4096
#define END_DELIM "\r\n"

void NICK(irc::Command *command);
void USER(irc::Command *command);
void PASS(irc::Command *command);
void PING(irc::Command *command);

void post_registration(irc::Command *command)
{
	command->reply(1, command->getClient().getprefix());
	command->reply(2, command->getClient().get_hostname());
	command->reply(3, command->getServer().getUpTime());
	command->reply(4, std::string("HLOIRC"), std::string("1.01"), std::string("aiwro"), std::string("Oov") + std::string("imnpt") + std::string("kl"));

}
void irc::Client::dispatch()
{
	ClientStatus last_status = status;

	if (last_status == DELETE)
		return ;
	std::vector<irc::Command *>remove = std::vector<irc::Command *>();
	for (std::vector<irc::Command *>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		if (last_status == PASSWORD)
		{
			if((*it)->getprefix() != "PASS")
				continue;
		}
		else if (last_status == REGISTER)
		{
			if ((*it)->getprefix()!= "NICK" && (*it)->getprefix() != "USER")
				continue;
		}
		if (command_function.count((*it)->getprefix()))
			command_function[(*it)->getprefix()]((*it));
		else if (DEBUG)
		{
			time_t t = time(0);
			struct tm * now = localtime( & t );
			char buffer[80];
			strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", now);
			std::cout << "[" << buffer << "] " << "Unknown command: " << (*it)->getprefix() << std::endl;
		}
		remove.push_back((*it));
	}
	for (std::vector<irc::Command *>::iterator it = remove.begin(); it != remove.end(); ++it)
		if (std::find(commands.begin(), commands.end(), *it) != commands.end())
		{
			commands.erase(std::find(commands.begin(), commands.end(), *it));
			delete *it;
		}

	if (last_status == REGISTER)
		if (nickname.length() && realname.length())
			status = ONLINE;

	if (last_status != status)
	{
		if (status == ONLINE)
			post_registration(*commands.begin());
		dispatch();
	}
}


irc::Client::Client(int _fd, sockaddr_in addr_) : command_function(), 
											 commands(),
											 waitingtoSend(),
											 nickname(""), 
											 username(""), 
											 realname(""), 
											 hostname(""),
											 buffer(),
											 status(PASSWORD),
											 fd(_fd)
											 
											 
										
											 
											
										
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
	client_ip_addr = inet_ntoa(addr_.sin_addr);
	char hostname[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&addr_, sizeof(addr_), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
	{
		std::cerr << "Error: getnameinfo" << std::endl;
		exit(1);
	}
	else 
		this->hostname = hostname;
	// remember to implement IP address function. 
	command_function["PASS"] = PASS;
	command_function["NICK"] = NICK;
	command_function["USER"] = USER;
	command_function["PING"] = PING;

	client_ip_addr = "";
	nickname_set = false;
	user_set = false;
	registered = false;
} 

irc::Client::Client( const irc::Client & rhs){*this = rhs;}
irc::Client::~Client(){close(fd);}

irc::Client&		irc::Client::operator=( Client const & rhs )
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
std::string	 		irc::Client::get_nickname(void) const {return nickname;}
std::string	 		irc::Client::get_username(void) const {return username;}
std::string	 		irc::Client::get_realname(void) const {return realname;}
std::string	 		irc::Client::get_hostname(void) const {return hostname;}
std::string	 		irc::Client::get_client_ip_addr(void) const {return client_ip_addr;}
bool 				irc::Client::get_nick_bool(void) {return nickname_set;}
bool 				irc::Client::get_user_bool(void) {return user_set;}
bool 				irc::Client::get_registered(void) {return registered;}
int 				irc::Client::get_fd() { return fd; }
time_t 				irc::Client::getLastPing() { return last_ping; }
irc::ClientStatus 	irc::Client::get_status() { return status; }
/*
std::string&		irc::Client::get_buffer(int std_n)
{
	if (std_n == 0)
		return buffer[0];
	if (std_n == 1)
		return buffer[1];
	return buffer[0];
}
*/
// Setters 
void 				irc::Client::set_nickname(std::string nick_n){nickname = nick_n;}
void 				irc::Client::set_username(std::string user_n){username = user_n;}
void 				irc::Client::set_realname(std::string real_n){realname = real_n;}
void 				irc::Client::set_hostname(std::string host_n){hostname = host_n;}
void 				irc::Client::set_client_ip_addr(std::string ip_addr){client_ip_addr = ip_addr;}
void 				irc::Client::set_nick_bool(bool new_val){nickname_set = new_val;}
void 				irc::Client::set_user_bool(bool new_val){user_set = new_val;}
void 				irc::Client::set_registered(bool new_val){registered = new_val;}
void 				irc::Client::set_status(ClientStatus stat){this->status = stat;}
/*
void				irc::Client::append_buffer(int std_n, const std::string& content )
{
	if (std_n == 0)
		buffer[0].append(content);
	if (std_n == 1)
		buffer[1].append(content);
}

void	irc::Client::clear_buffer(int std_n)
{
	if (std_n == 0)
		buffer[0].clear();
	if (std_n == 1)
		buffer[1].clear();
}
*/
std::string irc::Client::getprefix()
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

void irc::Client::receive_from(Server *server)
{
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
		buffer[size] = 0;
		this->buffer += buffer;
	}
	std::string delimiter(END_DELIM);
	size_t 		position;
	while ((position = buffer.find(delimiter)) != std::string::npos)
	{
		std::string message = buffer.substr(0, position);
		buffer.erase(0, position + delimiter.length());
		if (!message.length())
			continue;
		if (DEBUG)
		{
			time_t t = time(0);
			struct tm * now = localtime( & t );
			char buffer[80];
			strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", now);
			std::cout << "[" << buffer << "] " << fd << " < " << message << std::endl;
		}
		commands.push_back(new irc::Command(this, server, message));
	}
	dispatch();
}

void irc::Client::write(std::string message){waitingtoSend.push_back(message);}

void irc::Client::push()
{
	if (!waitingtoSend.size())
		return ;
	std::string buffer;
	for (std::vector<std::string>::iterator it = waitingtoSend.begin(); it != waitingtoSend.end(); ++it)
	{
		if (DEBUG)
		{
			time_t t = time(0);
			struct tm * now = localtime( & t );
			char buffer[80];
			strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", now);
			std::cout << "[" << buffer << "] " << fd << " > " << *it << std::endl;
		}
		buffer += *it + END_DELIM;
	}	
	waitingtoSend.clear();
	if (buffer.length())
		if (send(fd, buffer.c_str(), buffer.length(), 0) == -1)
		{
			std::cerr << "error: send()" << std::endl;
			exit(1);
		}
}
void irc::Client::sendTo(Client &toclient, std::string message) { toclient.write(":" + this->getprefix() + " " + message); }