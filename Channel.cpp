/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:56:40 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/31 11:44:02 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel()
{
    _creation_time = std::time(nullptr);
}

Channel::Channel(std::string name, Server &serv) : 
	_name(name), _key(""), _topic(""), _serv(&serv)
{
    _creation_time = std::time(nullptr);
    _topic_time    = std::time(nullptr);
}

Channel::~Channel() {}

bool Channel::is_user_in_channel(std::string nickname) const
{
	std::vector<std::pair<char, Client *> > clients = this->getClients();
	for(std::vector<std::pair<char, Client *> >::iterator it = clients.begin(); it != clients.end(); it++)
	{
		std::pair<char, Client *> current = *it;
		if (current.second->get_nickname() == nickname)
		{
			return true;
		}
	}
	return false;
}

const std::vector<std::pair<char, Client *> > &Channel::getClients() const {
	return _clients;
}

void Channel::send_to_all(std::string msg) const {
	for (size_t i = 0; i < this->getClients().size(); i++)
	{
		_serv->send_msg(msg, *this->getClients()[i].second);
	}
}

bool 	Channel::canSendMsg(Client const &owner) const 
{
	if (is_user_in_channel(owner.get_nickname()) == true)
	{
		//permissions
		return true;
	}
	return false;
}

void 	Channel::add_client( Client *new_client )
{
	if (this->is_user_in_channel(new_client->get_nickname())) {
		return; //already in channel, dont send anything back
	}
	if (!this->getClients().size()) //empty
	{
		this->_clients.push_back(std::make_pair('O', new_client)); //assume oper here
	}
	else
	{
		this->_clients.push_back(std::make_pair('w', new_client));
	}
}

