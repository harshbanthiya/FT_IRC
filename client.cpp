/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:08:30 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/23 19:08:28 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <fcntl.h>
#include "Utils.hpp"

#define MAX_SIZE 4096
#define END_DELIM "\r\n"


irc::Client::Client(int _fd, std::string host) : socket_fd(_fd), hostname(host), pass_set(false), registered(false), 
{}

irc::Client::~Client(){}

std::string& irc::Client::buffer() {return (this->buffer);}

bool 		irc::Client::is_passed() const {return (this->pass_set);}
bool 		irc::Client::is_registered() const {return(this->registered);}

void 		irc::Client::set_passed() {this->pass_set = true;}
void 		irc::Client::set_registered(){this->registered = true;}

std::string	toUpper(std::string const & str)
{
	std::string result;
	for (unsigned int i=0; i<str.length(); i++)
		result += std::toupper(str[i]);
	return (result);
}

bool				irc::Client::operator==(std::string const & nick) const
{ return (toUpper(this->nickname) == toUpper(nick)); }

bool				irc::Client::operator==(Client const & other) const
{ return (*this == other.nickname); }

// Getters
std::string	 		irc::Client::get_nickname(void) const {return nickname;}
std::string	 		irc::Client::get_username(void) const {return username;}
std::string	 		irc::Client::get_realname(void) const {return realname;}
std::string	 		irc::Client::get_hostname(void) const {return hostname;}




time_t 				irc::Client::getLastPing() { return last_ping; }


// Setters 
void 				irc::Client::set_nickname(std::string nick_n){nickname = nick_n;}
void 				irc::Client::set_username(std::string user_n){username = user_n;}
void 				irc::Client::set_realname(std::string real_n){realname = real_n;}
void 				irc::Client::set_hostname(std::string host_n){hostname = host_n;}

