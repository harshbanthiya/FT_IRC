/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:56:40 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/07 11:16:44 by olabrecq         ###   ########.fr       */
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
    _creation_time	= std::time(nullptr);
    _topic_time		= std::time(nullptr);
	_limit			= INT32_MAX;		  
}

Channel::Channel(std::string name, std::string key, Server &server) :
	_name(name), _key(key), _topic(""), _serv(&server)
{
	if (!key.empty())
		_modes = "k";
	_creation_time	= std::time(nullptr);
    _topic_time		= std::time(nullptr);
	_limit			= INT32_MAX;			
}

Channel::Channel(Channel const &ch) :
	_name(ch._name), _key(ch._key), _topic(ch._topic), _serv(ch._serv), _clients(ch._clients)
{
	_creation_time	= ch._creation_time;
	_topic_time		= ch._topic_time;
	_limit			= INT32_MAX;
	_modes 			= ch._modes;
}

Channel::~Channel() {}

//================================ 


bool				Channel::empty(){return (_clients.empty());}

void 	Channel::ban(Client &owner, std::string nick)
{	
	if (!this->is_operator(owner))
		return (_serv->getHandler().get_replies(ERR_CHANOPRIVSNEEDED, owner, _name));
	if ((_ban_list.find(nick) != _ban_list.end()))
		return ;
	_ban_list.insert(nick);
	std::string msg = ":" + owner.get_nickname() + "!" + owner.get_username() + '@' + owner.get_hostname() +
						" MODE " + _name + " +b :" + nick + "!*@*" + END_DELIM;
	if (this->_modes.find('b') == std::string::npos)
		_modes += "b";
	this->send_to_all(msg);
}

void 	Channel::unBan(Client &owner, std::string nick)
{
	if (!this->is_operator(owner))
		return (_serv->getHandler().get_replies(ERR_CHANOPRIVSNEEDED, owner, _name));
	if ((_ban_list.find(nick) != _ban_list.end()))
		return ;
	_ban_list.erase(nick);
	std::string msg = ":" + owner.get_nickname() + "!" + owner.get_username() + '@' + owner.get_hostname() +
						" MODE " + _name + " -b :" + nick + "!*@*" + END_DELIM;
	this->send_to_all(msg);
	if (_ban_list.empty())
		this->delete_mode('b');
}

void		Channel::delete_mode(char mode)
{
	int pos;
	if ((pos = this->_modes.find(mode)) == -1)
		return ;
	this->_modes = this->_modes.substr(0, pos) + this->_modes.substr(pos+1);
}

void 	Channel::add_mode(char mode)
{
	if (this->_modes.find(mode) == std::string::npos)
		_modes += mode;
}

bool 	Channel::add_mode(Client &owner, char m, char mode, std::string params)
{
	if (!mode)
		mode = '+';
	switch(m)
	{
		case 'b':
			return (mode_ban(owner, mode, params));
		case 'i':
			return (mode_invite(owner, mode));
		case 'o':
			return (mode_operator(owner, mode, params));
	}
	_serv->getHandler().get_replies(ERR_UNKNOWNMODE, owner, std::string(1, m));
	return (false);
}

bool 				Channel::mode_invite(Client &owner, char mode)
{
	std::string msg;
	if (!this->is_operator(owner))
		_serv->getHandler().get_replies(ERR_CHANOPRIVSNEEDED, owner, _name);
	else if (mode == '-' && _modes.find('i') != std::string::npos)
	{
		msg = ":" + owner.get_nickname() + "!" + owner.get_username() + '@' + owner.get_hostname() + 
				" MODE " + _name + " -i" + END_DELIM;
		this->send_to_all(msg);
		this->delete_mode('i');
 	}
	else if (_modes.find('i') == std::string::npos)
	{
		msg = ":" + owner.get_nickname() + "!" + owner.get_username() + '@' + owner.get_hostname() + 
				" MODE " + _name + " +i" + END_DELIM;
		this->send_to_all(msg);
		if (this->_modes.find('i') == std::string::npos)
			_modes += "i";
	}
	return (false);
}

bool	Channel::mode_ban(Client &owner, char mode, std::string params)
{
	std::string 	msg;
	(void)owner;
	if (params == "" && mode == '-') {
		
		return false;
	else if (params == "")
	{
		send_ban_list(owner);
		return (false);
	}
	else if (mode == '+')
		ban(owner, params);
	else 
		unBan(owner, params);
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

bool				Channel::is_invited(Client const &owner) const
{
	return (_invite_list.find(owner.get_nickname()) != _invite_list.end());
}

bool				Channel::is_invited(std::string const &owner) const
{
	return (_invite_list.find(owner) != _invite_list.end());
}

bool				Channel::is_banned(Client const &owner) const
{
	return (_ban_list.find(owner.get_nickname()) != _ban_list.end());
}

bool				Channel::is_banned(std::string const &owner) const
{
	return (_ban_list.find(owner) != _ban_list.end());
}


bool 	Channel::can_join(Client const &owner) const
{
	
	if (this->is_user_in_channel(owner))
	{
		_serv->getHandler().get_replies(ERR_USERONCHANNEL, owner, owner.get_nickname() + " " + _name);
		return (false);
	}
	if (_modes.find('i') != std::string::npos && !this->is_invited(owner))
	{
		_serv->getHandler().get_replies(ERR_INVITEONLYCHAN, owner, _name);
		return false;
	}
	
	if (is_banned(owner) && (_invite_list.find(owner.get_nickname()) == _invite_list.end()))
	{
		_serv->getHandler().get_replies(ERR_BANNEDFROMCHAN, owner, _name);
		return (false);
	}
	return (true);
}

bool 	Channel::can_send_msg(Client const &owner) const
{
	if (_modes.find('n') != std::string::npos && !this->is_user_in_channel(owner))
	{
		_serv->getHandler().get_replies(ERR_CANNOTSENDTOCHAN, owner, _name);
		_serv->getHandler().get_replies(ERR_NOTONCHANNEL, owner, _name);
		return (false);		
	}
	if (this->is_banned(owner))
	{
		_serv->getHandler().get_replies(ERR_BANNEDFROMCHAN, owner, _name);
		return (false);
	}
	return (true);
}

void 	Channel::send_ban_list(Client &owner) const
{
	for(std::set<std::string>::iterator i = _ban_list.begin(); i != _ban_list.end(); i++)
	{
		std::string msg = _name + " " + *i;
		_serv->getHandler().get_replies(RPL_BANLIST, owner, msg);
	}
	_serv->getHandler().get_replies(RPL_ENDOFBANLIST, owner, _name);
	
}

void 	Channel::add_client( Client &new_client, std::string key, char status = 0)
{
	
	if (!this->can_join(new_client)) 
		return ;
	if (_key == key)
	{
		new_client.add_channel(_name);
		_invite_list.erase(new_client.get_nickname());
		_clients.push_back(std::pair<char, Client*>(status, &new_client));
		std::string msg = ":" + new_client.get_nickname() + "!" + new_client.get_username() + '@' + new_client.get_hostname() + " JOIN :" + _name + END_DELIM;
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

void 	Channel::send_to_all(std::string msg, std::string sender) const 
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (sender == "" || sender != _clients[i].second->get_nickname())
			_serv->send_msg(msg, *(_clients[i].second));
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

bool Channel::is_user_in_channel(Client const &user) const
{
	return (is_user_in_channel(user.get_nickname()));
}

bool Channel::is_user_in_channel(std::string nickname) const
{
	for (u_int i = 0; i < this->_clients.size() ; i++)
	{
		if (*_clients[i].second == nickname)
			return (true);
	}
	return false;
}

bool 	Channel::remove_client(Client &cl)
{
	return (remove_client(cl.get_nickname()));
}

bool 	Channel::remove_client(std::string const &nick)
{
	if(!this->is_user_in_channel(nick))
		return false;
	u_int i	 = 0;
	for (; i < _clients.size(); i++)
	{
		if (*(_clients[i].second) == nick)
			break ;
	}
	(*_clients[i].second).remove_channel(_name);
	this->_clients.erase(this->_clients.begin() + i);
	return (true);
}

bool		Channel::is_operator(Client const &user) const
{
	u_int		i = 0;
	for (; i < _clients.size(); i++)
	{
		if (_clients[i].first == '@' && user == *_clients[i].second)
			return (true);
	}
	return (false);
}

bool		Channel::is_operator(std::string const &user) const
{
	u_int		i = 0;
	for (; i < _clients.size(); i++)
	{
		if (_clients[i].first == '@' && user == _clients[i].second->get_nickname())
			return (true);
	}
	return (false);
}

void 	Channel::kick(Client &client, std::list<std::string> &clients, std::string msg)
{
	if (!this->is_user_in_channel(client))
	{
		_serv->getHandler().get_replies(ERR_NOTONCHANNEL, client, _name);
		return ;
	}
	if (!is_operator(client))
	{
		_serv->getHandler().get_replies(ERR_CHANOPRIVSNEEDED, client, _name);
		return ;
	}
	for (std::list<std::string>::iterator i = clients.begin(); i != clients.end(); i++)
	{
		if (this->is_user_in_channel(*i))
		{
			msg = ":" + client.get_nickname() + "!" +  client.get_username() + " KICK "+ _name + " " + *i + " :" + msg + END_DELIM;
			send_to_all(msg);
			this->remove_client(*i);
		}
		else 
			_serv->getHandler().get_replies(ERR_USERNOTINCHANNEL, client, *i + " " + _name);
	}
}

void 	Channel::invite(Client &owner, std::string nick)
{
	if (!this->is_user_in_channel(owner))
		return (_serv->getHandler().get_replies(ERR_NOTONCHANNEL, owner, _name));
	if (!this->is_operator(owner))
		return (_serv->getHandler().get_replies(ERR_CHANOPRIVSNEEDED, owner, _name));
	if (this->is_user_in_channel(nick))
		return (_serv->getHandler().get_replies(ERR_USERONCHANNEL, owner, nick + " " + _name));
	_invite_list.insert(nick);
	_serv->getHandler().get_replies(RPL_INVITING, owner, nick + " " + _name);
	std::string msg = ":" + owner.get_nickname() + "!" + owner.get_username() + '@' + owner.get_hostname()
						+ " INVITE " + nick + " :" + _name + END_DELIM;
	_serv->send_msg(msg, nick);
}

std::string const & Channel::get_modes() const { return (this->_modes); }
std::string 		Channel::get_name(bool ck) const
{
	if (!ck || _modes.find('s') == std::string::npos)
		return (_name);
	else 
		return ("*");
}
std::string 		Channel::get_topic()const {return (_topic);}
std::string 		Channel::get_key()const{return (_key);}
std::string 		Channel::get_creation_time()const{return (std::to_string(_creation_time));}


const std::vector<std::pair<char, Client *> >  &Channel::get_client_list() const {
	return _clients;
}

const std::set<std::string>  &Channel::get_ban_list() const {
	return _ban_list;
}

