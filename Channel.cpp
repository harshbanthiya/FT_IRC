/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:56:40 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/31 15:35:45 by olabrecq         ###   ########.fr       */
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

std::string const & Channel::get_modes() const { return (this->_modes); }

std::string 	Channel::get_creation_time() const{ return(std::to_string(_creation_time));}

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

bool 	Channel::add_mode(Client &owner, char m, char mode, std::string params)
{
	if (!mode)
		mode = '+';
	switch(m)
	{
		case 'b':
			return (mode_ban(owner, mode, params));
		// case 'i':
			// return (mode_invite(owner, mode));
		case 'o':
			return (mode_operator(owner, mode, params));
	}
	_serv->getHandler().get_replies(ERR_UNKNOWNMODE, owner, std::string(1, m));
	return (false);
}


// MODES 



bool	Channel::mode_ban(Client &owner, char mode, std::string params)
{
	std::string 	msg;
	(void)owner;
	if (params == "" && mode == '-') {
		
		return false;
	}
	else if (params == "") {
		return (false);
	}
	else if (mode == '+') {
		return (true);
	}
	return true;
	//	ban(owner, params);   Need to make a ban list to implement these 
	// else 
	//	unban(owner, params);
}



bool				Channel::mode_operator(Client &owner, char mode, std::string params)
{
	std::string msg;
	if (!this->is_operator(owner))
		_serv->getHandler().get_replies(ERR_CHANOPRIVSNEEDED, owner, _name);
	else if (params == "")
		return false;
	else 
	{
		size_t i = 0;
		for (; i< _clients.size(); i++)
			if (*_clients[i].second == params)
				break;
		if (i < _clients.size())
		{
			if (mode == '+')
				_clients[i].first = '@';
			else
				_clients[i].first = '\0';
			msg = ":" + owner.get_nickname() + "!" +  owner.get_username() + '@' + owner.get_hostname() + 
						" MODE " + _name + " " + mode +"o " + params + END_DELIM;
			this->send_to_all(msg);
		}
		else
			_serv->getHandler().get_replies(ERR_NOSUCHNICK, owner, params);
	}
	return true;
}


bool		Channel::is_operator(Client const &user) const
{
	u_int		i = 0;
	for (; i < _clients.size(); i++)
	{
		if (_clients[i].first == '@' && user == _clients[i].second->get_nickname())
			return (true);
	}
	return (false);
}
