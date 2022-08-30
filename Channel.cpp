/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:56:40 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/30 09:19:18 by olabrecq         ###   ########.fr       */
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

bool Channel::is_user_in_channel(std::string nickname)
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


