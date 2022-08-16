/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:08:30 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/16 16:06:32 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#define MAX_SIZE 4096
#define END_DELIM "\r\n"

Client::Client(int _fd, sockaddr_in6 addr_) : fd(_fd), client_addr(addr_), nickname(""), username(""), realname(""), hostname("")
{
	// remember to implement IP address function. 
	client_ip_addr = "";
	nickname_set = false;
	user_set = false;
	registered = false;
} 

Client::Client( const Client & rhs){*this = rhs;}
Client::~Client(){}

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