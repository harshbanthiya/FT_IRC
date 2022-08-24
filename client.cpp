/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:08:30 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/24 13:27:43 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"



Client::Client(int _fd, std::string host) : socket_fd(_fd), hostname(host), pass_set(false), registered(false)
{}

Client::~Client(){}

std::string& Client::buffer() {return (this->_buffer);}

bool 		Client::is_passed() const {return (this->pass_set);}
bool 		Client::is_registered() const {return(this->registered);}

void 		Client::set_passed() {this->pass_set = true;}
void 		Client::set_registered(){this->registered = true;}

std::string	toUpper(std::string const & str)
{
	std::string result;
	for (unsigned int i=0; i<str.length(); i++)
		result += std::toupper(str[i]);
	return (result);
}

bool				Client::operator==(std::string const & nick) const
{ return (toUpper(this->nickname) == toUpper(nick)); }

bool				Client::operator==(Client const & other) const
{ return (*this == other.nickname); }

// Getters
std::string	 		Client::get_nickname(void) const {return nickname;}
std::string	 		Client::get_username(void) const {return username;}
std::string	 		Client::get_realname(void) const {return realname;}
std::string	 		Client::get_hostname(void) const {return hostname;}
int					Client::getSocket(void) const{ return (this->socket_fd); };



time_t 				Client::getLastPing() { return last_ping; }


// Setters 
void 				Client::set_nickname(std::string nick_n){nickname = nick_n;}
void 				Client::set_username(std::string user_n){username = user_n;}
void 				Client::set_realname(std::string real_n){realname = real_n;}
void 				Client::set_hostname(std::string host_n){hostname = host_n;}

