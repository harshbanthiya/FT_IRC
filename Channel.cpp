/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:56:40 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/01 13:28:41 by hbanthiy         ###   ########.fr       */
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

Channel::Channel(std::string name, std::string key, Server &server) :
	_name(name), _key(key), _topic(""), _serv(&server)
{
	if (!key.empty())
		_modes = "k";
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

std::string		Channel::get_str_clients()const
{
	std::string 	s = "";
	for (size_t i = 0; i < _clients.size() ; i++)
	{
		if (_clients[i].first)
			s += (_clients[i].first) + (_clients[i].second)->get_nickname() + " ";
		else 
			s += (_clients[i].second)->get_nickname() + " ";
	}
	return (s);
}
bool 	Channel::canSendMsg(Client const &owner) const{
	if (this->is_user_in_channel(owner.get_nickname()) == true)
	{
		//permissions
		return true;
	}
	return false;
}
std::string 	Channel::get_name(bool ck) const
{
		if (!ck || _modes.find('s') == std::string::npos)
			return _name;
		else
			return "*";
}

void 	Channel::add_client( Client &new_client, std::string key, char status = 0)
{
	if (this->is_user_in_channel(new_client.get_nickname())) {
		return ; //already in channel, dont send anything back
	}

	if (key == _key)
	{
		new_client.add_channel(_name);
		_clients.push_back(std::pair<char, Client*>(status, &new_client));
		std::string msg = ":" + new_client.get_nickname() + "!" + new_client.get_username() + '@' + new_client.get_hostname() + "JOIN" + _name + END_DELIM;
		this->send_to_all(msg);
		if (!_topic.empty())
			_serv->getHandler().get_replies(RPL_TOPIC, new_client, _name);
		_serv->getHandler().get_replies(RPL_NAMREPLY, new_client, "= " + _name + " :" + this->get_str_clients());
		_serv->getHandler().get_replies(RPL_ENDOFNAMES, new_client, _name);
		return ;
	}
	else 
		_serv->getHandler().get_replies(ERR_BADCHANNELKEY, new_client, _name);
	
}

bool 	Channel::add_mode(Client &owner, char m, char mode, std::string params)
{
	if (!mode)
		mode = '+';
	switch(m)
	{
		case 'b':
			return (mode_ban(owner, mode, params));
		//case 'i':
		//	return (mode_invite(owner, mode));
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

	if (params == "" && mode == '-')
		return false;
	else if (params == "")
		return (false);
	std::cout << owner.get_nickname(); // test
	//else if (mode == '+')
	//	ban(owner, params);   Need to make a ban list to implement these 
	// else 
	//	unban(owner, params);
	return (true);
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