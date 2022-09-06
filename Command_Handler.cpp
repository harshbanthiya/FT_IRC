/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_Handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 13:37:38 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/06 14:07:15 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_Handler.hpp"
#include "Server.hpp"

CommandHandler::CommandHandler(Server &_server): serv(_server)
{
	this->handlers["PASS"] = &CommandHandler::handle_pass;
	this->handlers["NICK"] = &CommandHandler::handle_nick;
	this->handlers["USER"] = &CommandHandler::handle_user;
	this->handlers["MOTD"] = &CommandHandler::handle_motd;
	this->handlers["PING"] = &CommandHandler::handle_ping;
	this->handlers["JOIN"] = &CommandHandler::handle_join;
	this->handlers["PRIVMSG"] = &CommandHandler::handle_privmsg;
	this->handlers["KICK"] = &CommandHandler::handle_kick;
	this->handlers["MODE"] = &CommandHandler::handle_mode;
	this->handlers["INVITE"] = &CommandHandler::handle_invite;
	this->handlers["TIME"] = &CommandHandler::handle_time;
	//this->handlers["WHO"] = &CommandHandler::handle_who;
	// this->handlers["DIE"] = &CommandHandler::handle_user;
	/*
		LUSERS
		PART
		AWAY
		QUIT
		WHO
		NAMES
		LIST
		TOPIC
	*/
}

void 	CommandHandler::parse_cmd(std::string cmd_line)
{
	//std::cout << cmd_line << std::endl;
	if (cmd_line.empty())
		return ;
	int pos = cmd_line.find(" ");
	command = toUpper(cmd_line.substr(0, pos));
	cmd_line.erase(0, (pos != -1) ? pos + 1 : pos);
	parameters.clear();
	while (!cmd_line.empty())
	{
		if (cmd_line[0] == ':')
		{
			cmd_line.erase(0, 1);
			if (cmd_line.empty())
			{
				parameters.push_back("");
				break ;
			}
			parameters.push_back(cmd_line);
			cmd_line.erase(0);
		}
		else 
		{
			pos = cmd_line.find(" ");
			parameters.push_back(cmd_line.substr(0, pos));
			cmd_line.erase(0, (pos != -1) ? pos + 1 : pos);
		}
	}
}

void CommandHandler::handle(std::string cmd_line, Client &owner)
{
	parse_cmd(cmd_line);
	if (command.empty())
		return ;
	if (!owner.is_passed() && command != "PASS")
		return ;
	else if (owner.is_passed() &&  !owner.is_registered() && command != "NICK" && command != "USER")
		return get_replies(ERR_NOTREGISTERED, owner, command);
	if (handlers.find(command) == handlers.end())
		get_replies(ERR_UNKNOWNCOMMAND, owner, command);
	else 
		(*this.*(this->handlers[this->command]))(owner);
} 

void	CommandHandler::handle_pass(Client &owner)
{
	if (!parameters.size() || parameters.front() == "")
		return get_replies(ERR_NEEDMOREPARAMS, owner, command);
	if(owner.is_registered())
		return get_replies(ERR_ALREADYREGISTERED, owner);
	if (serv.check_pass(this->parameters.front()))
		owner.set_passed();
	else 
		get_replies(ERR_PASSWDMISMATCH, owner);
}

void	CommandHandler::handle_nick(Client &owner)
{
	if (!parameters.size() || parameters.front() == "")
		return get_replies(ERR_NONICKNAMEGIVEN, owner);
	
	std::string& nick = this->parameters.front();
	std::vector<Client *> const &clients = this->serv.get_all_clients();
	for(u_int i = 0; i < clients.size(); i++)
	{
		if (*clients[i] == nick)
			return get_replies(ERR_NICKNAMEINUSE, owner, nick);
	}
	std::string old_nick = owner.get_nickname();
	owner.set_nickname(nick);
	if (old_nick != "")
	{
		std::string msg = ":" + old_nick + "!" + owner.get_username() + "@" + owner.get_hostname() + " NICK: " + owner.get_nickname() + END_DELIM;
		this->serv.send_msg(msg, owner);
	}
	if (!owner.is_registered() && !owner.get_username().empty())
		welcomescreen(owner);
	
}

void 	CommandHandler::handle_user(Client &owner)
{
	if (parameters.size() != 4)
		return get_replies(ERR_NEEDMOREPARAMS, owner, command);
	if(owner.is_registered())
		return get_replies(ERR_ALREADYREGISTERED, owner);
	std::string username = parameters.front();
	if (username.empty())
		return get_replies(ERR_NEEDMOREPARAMS, owner, command);
	std::string realname = parameters.back();
	owner.set_username(username);
	owner.set_realname(realname);
	if (!owner.get_nickname().empty())
		welcomescreen(owner);
}

void	CommandHandler::handle_motd(Client &owner)
{
	if (command == "MOTD" && parameters.size() && parameters.front() != SERV_NAME)
		return (get_replies(ERR_NOSUCHSERVER, owner, parameters.front()));
	std::vector<std::string> motd = this->serv.get_motd();
	if (!motd.size())
		return get_replies(ERR_NOMOTD, owner);
	get_replies(RPL_MOTDSTART, owner);
	for (u_int i = 0; i < motd.size(); i++)
		get_replies(RPL_MOTD, owner, motd[i]);
	get_replies(RPL_ENDOFMOTD, owner);	
}

void 	CommandHandler::handle_ping(Client &owner)
{
	if (!parameters.size() || parameters.front() == "")
		return get_replies(ERR_NEEDMOREPARAMS, owner, command);
	std::string msg = ":" + SERV_NAME + " PONG " + SERV_NAME + " :" + parameters.front() + END_DELIM;
	this->serv.send_msg(msg, owner);
}

 void 	CommandHandler::handle_privmsg(Client &owner)
 {
 	if (!this->parameters.size() || this->parameters.front() == "")
 		return get_replies(ERR_NORECIPIENT, owner, this->command);
 	if (this->parameters.size() == 1)
 		return get_replies(ERR_NOTEXTTOSEND, owner);

 	std::string targets = parameters.front(); //should be username or channel
 	std::list<std::string>::iterator it = ++this->parameters.begin();
 	std::string text = " :" + *it;
 	for (++it; it != parameters.cend(); ++it)
 		text += " " + *it;
 	std::string head = ":" + owner.get_nickname() + "!" + owner.get_username() + "@" + owner.get_hostname() + " PRIVMSG " ;
 	while (!targets.empty())
 	{
 		int pos = targets.find(",");
 		std::string curr_target = targets.substr(0, pos);
		std::string msg = head + curr_target + text + END_DELIM;
 		int rv;
 		if (curr_target[0] == '#')
			rv = this->serv.send_msg(msg, curr_target, owner); // put all the logic in the overloaded function
 		else
 			rv = this->serv.send_msg(msg, curr_target);
 		if (rv == ERR_NOSUCHNICK)
 			get_replies(rv, owner, curr_target);
 		targets.erase(0, (pos != -1) ? pos + 1 : pos);
 	}
}

void CommandHandler::handle_join(Client &owner)
{

	if (parameters.empty())
		return get_replies(ERR_NEEDMOREPARAMS, owner, command);
	
	int 					pos;
	std::list<std::string> 	names;
	std::list<std::string> 	keys;

	while (parameters.front() != "")
	{
		pos = parameters.front().find(",");
		names.push_back(parameters.front().substr(0, pos));
		parameters.front().erase(0, (pos != -1) ? pos + 1 : pos);
	}
	parameters.pop_front();
	if (!parameters.empty())
	{
		while(parameters.front() != "")
			{
				pos = parameters.front().find(",");
				keys.push_back(parameters.front().substr(0, pos));
				parameters.front().erase(0, (pos != -1) ? pos + 1 : pos);
			}
		parameters.pop_front();
	}
	if (names.front()[0] != '#')
		return get_replies(ERR_NOSUCHCHANNEL, owner, names.front());
	while (!names.empty())
	{
		char 	stat  = 0;
		if (!serv.check_channel(names.front())) 
		{
			Channel new_chan(names.front(), keys.front(), serv);
			serv.add_channel(new_chan);
			stat = '@';
		}
		Channel &chan = serv.get_channel(names.front());
		chan.add_client(owner, keys.front(), stat);
		if (!keys.empty())
			keys.pop_front();
		names.pop_front();
	}
}

void 	CommandHandler::handle_kick(Client &owner)
{
	if (parameters.size() < 2)
		return (get_replies(ERR_NEEDMOREPARAMS, owner, "KICK"));
	std::list<std::string> 	channels;
	std::list<std::string> 	clients;
	int 					pos;


	while (parameters.front() != "")
	{
		pos = parameters.front().find(",");
		channels.push_back(parameters.front().substr(0, pos));
		parameters.front().erase(0, (pos != 1) ? pos + 1 : pos);
	}
	parameters.pop_front();
	while (parameters.front() != "")
	{
		pos = parameters.front().find(",");
		clients.push_back(parameters.front().substr(0, pos));
		parameters.front().erase(0, (pos != -1) ? pos + 1 : pos);
	}
	parameters.pop_front();
	while (!channels.empty())
	{
		if (serv.check_channel(channels.front()))
		{
			Channel &chan = serv.get_channel(channels.front());
			if (!parameters.empty())
				chan.kick(owner, clients, parameters.front());
			else 
				chan.kick(owner, clients);
			if (chan.empty())
				serv.remove_channel(chan.get_name());
			channels.pop_front();
		}
		else 
			get_replies(ERR_NOSUCHCHANNEL, owner, channels.front());
	}
}

void CommandHandler::handle_mode(Client &owner)
{
	if (!this->parameters.size() || this->parameters.front() == "")
		return (get_replies(ERR_NEEDMOREPARAMS, owner, this->command));
	std::string target = this->parameters.front();
	if (target[0] == '#') // Channel Modes 
	{
		if (!serv.check_channel(target))
			return (get_replies(ERR_NOSUCHCHANNEL, owner, target));
		Channel &ch = serv.get_channel(target);
		if (this->parameters.size() == 1)
		{
			get_replies(RPL_CHANNELMODEIS, owner, target + " +" + serv.get_channel(target).get_modes());
			get_replies(RPL_CREATIONTIME, owner, target +  " " + serv.get_channel(target).get_creation_time());
			return ;
		}
		parameters.pop_front();
		std::string mode = parameters.front();
		parameters.pop_front();
		char type = (mode[0] == '-' || mode[0] == '+') ? mode[0] : 0;
		for(size_t i = (type != 0); i < mode.size(); i++)
		{
			if (ch.add_mode(owner, mode[i], type, parameters.front()))
				parameters.pop_front();
		}
	}
	else // USER MODE 
	{
		std::vector<Client *> clients = serv.get_all_clients();
		uint i = 0;
		for (; i < clients.size() && clients[i]->get_nickname() != target; i++);
		if (i == clients.size())
			return (get_replies(ERR_NOSUCHNICK, owner, target));
		if (owner.get_nickname() != target)
			return (get_replies(ERR_USERSDONTMATCH, owner));
		if (parameters.size() == 1)
			return (get_replies(RPL_UMODEIS, owner, target));
		
		std::string modestring = *(++(this->parameters.begin()));
		std::string msg = " ";
		for (i = 0; i < modestring.length(); i++)
		{
			char mode = modestring[i];
			if (mode == '+' || mode == '-')
				continue ;
			if (UMODES.find(mode) == std::string::npos)
				get_replies(ERR_UMODEUNKNOWNFLAG, owner);
			else if (i && modestring[i - 1] == '-')
			{
				owner.del_mode(mode);
				msg += "-";
				msg += mode;
			}
			else 
			{
				owner.add_mode(mode);
				msg += "+";
				msg += mode;
			}
		}
		if (msg != " ")
		{
			msg = ":" + owner.get_nickname() + "!" + owner.get_username() + "@" + owner.get_hostname() + " MODE " + owner.get_nickname() + msg + END_DELIM;
			this->serv.send_msg(msg, owner);
		}
	}

}

void 	CommandHandler::handle_invite(Client &owner)
{
	std::string msg = "";
	std::string nick;

	if (parameters.size() < 2 || parameters.back() == "")
		return (get_replies(ERR_NEEDMOREPARAMS, owner, command));
	nick = parameters.front();
	if(!serv.user_exists(parameters.front()))
		return(get_replies(ERR_NOSUCHNICK, owner, parameters.front()));
	parameters.pop_front();
	if (!serv.check_channel(parameters.front()))
		return (get_replies(ERR_NOSUCHCHANNEL, owner, parameters.front()));
	Channel &ch = serv.get_channel(parameters.front());
	ch.invite(owner, nick);
}


void CommandHandler::welcomescreen(Client &target)
{
	target.set_registered();
	get_replies(RPL_WELCOME, target);
	get_replies(RPL_YOURHOST, target);
	get_replies(RPL_CREATED, target, this->serv.getcreatedTime());
	get_replies(RPL_MYINFO, target);
	handle_motd(target);
}

void CommandHandler::handle_time(Client &target)
{
	std::time_t result = std::time(nullptr);
    
	std::string msg = std::string(std::asctime(std::localtime(&result))); 
	get_replies(RPL_TIME, target, msg);
}

void 	CommandHandler::get_replies(int code, Client const &owner, std::string extra) const
{
	std::string msg = ":" + SERV_NAME + " ";
	if (code < 10)
	{
		msg += "00";
		msg += code + '0';
	}
	else 
		msg += std::to_string(code);
	
	msg += " " + owner.get_nickname() + " ";

	switch (code)
	{
		case RPL_WELCOME:
		 	msg += ":Welcome to the Internet Relay Network " ;
			msg += owner.get_nickname() + "!" + owner.get_username() + "@" + owner.get_hostname();
			break ;
		case RPL_YOURHOST: 
			msg += ":Your host is " + SERV_NAME + ", running version IRC1.0";
			break;
		case RPL_CREATED: 
			msg += ":This server was created " + extra;
			break;
		case RPL_MYINFO:
			msg += SERV_NAME + " IRC1.0 " + UMODES + " " + CMODES;
			break ;
		case RPL_UMODEIS:
			msg += owner.get_modes();
			break;
		case RPL_AWAY:
			msg += extra;
			break ;
		case RPL_ENDOFWHO:
			msg += extra + " :End of WHO list";
			break;
		case RPL_CHANNELMODEIS: 
			msg += extra;
			break;
		case RPL_CREATIONTIME:
			msg += extra;
			break;
		case RPL_TOPIC:
			msg += extra;
			break;
		case RPL_INVITING:
			msg += extra;
			break;
		case RPL_WHOREPLY:
			msg += extra + " :<hopcount> <real name>";
			break;
		case RPL_NAMREPLY:
			msg += extra + " :Channel Create";
			break;
		case RPL_ENDOFNAMES:
			msg += extra + " :End of NAMES list";
			break;
		case RPL_BANLIST:
			msg += extra + " :End of channel ban list.";
			break;
		case RPL_ENDOFBANLIST:
			msg += extra + " :End of /NAMES list";
			break;
		case RPL_MOTD:
			msg += ":" + extra;
			break;
		case RPL_MOTDSTART:
			msg += ":- " + SERV_NAME + " Message of the day - ";
			break;
		case RPL_ENDOFMOTD:
			msg += ":End of /MOTD command.";
			break;
		case ERR_NOSUCHNICK:
			msg += extra + " :No such nick";
			break;
		case ERR_NOSUCHSERVER:
			msg += extra + " :No such server";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += extra + " :No such channel";
			break;
		case ERR_CANNOTSENDTOCHAN:
			msg += extra + " :Cannot send to channel";
			break;
		case ERR_NORECIPIENT:
			msg += ":No recepient given ( " + extra + ")";
			break;
		case ERR_NOTEXTTOSEND:
			msg += ":No text to send";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg += extra + " :Unknown command";
			break;
		case ERR_NOMOTD:
			msg += ":MOTD File is missing";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += ":No nickname given";
			break;
		case ERR_NICKNAMEINUSE:
			msg += extra + " :Nickname is already in use"; 
			break;
		case ERR_USERNOTINCHANNEL:
			msg += extra + " :They aren't on that channel";
			break;
		case ERR_NOTONCHANNEL:
			msg += extra + " :You're not on that channel";
			break;
		case ERR_USERONCHANNEL:
			msg += extra + " :is already on channel";
			break;
		case ERR_NOTREGISTERED:
			msg += extra + " :You have not registered";
			break;
		case ERR_NEEDMOREPARAMS:
			msg += extra + " :Not enough parameters";
			break;
		case ERR_ALREADYREGISTERED:
			msg += ":You may not re register";
			break;
		case ERR_PASSWDMISMATCH:
			msg += ":Password incorrect";
			break;
		case ERR_UNKNOWNMODE:
			msg += extra + " :is unknown mode char to me";
			break;
		case ERR_INVITEONLYCHAN:
			msg += extra + " :Cannot join channel (+i)";
			break;
		case ERR_BANNEDFROMCHAN:
			msg += extra + " :Cannot join channel (+b)";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += extra + " :You're not channel operator";
			break;
		case ERR_BADCHANNELKEY:
			msg += extra + " :Cannot join channel (+k)";
			break;
		case ERR_UMODEUNKNOWNFLAG:
			msg += ":Unknown MODE flag";
			break;
		case ERR_USERSDONTMATCH:
			msg += ":Cant change mode for other users";
			break;
	
		case RPL_TIME:
			msg += extra;
			break;
		case RPL_ADMINME:
			msg += extra;
			break;
		case RPL_ADMINLOC1:
			msg += extra;
			break;
		case RPL_ADMINLOC2:
			msg += extra;
			break;
		case RPL_ADMINEMAIL:
			msg += extra;
			break;
    }
	msg += END_DELIM;
	this->serv.send_msg(msg, owner);
}


/*
void CommandHandler::handle_admin(Client &target)
{
	get_replies(RPL_ADMINME, target, " :Administrative info");
	get_replies(RPL_ADMINLOC1, target, "Name     - The Routing Team");
	get_replies(RPL_ADMINLOC2, target, "Nickname - #Routing");
	get_replies(RPL_ADMINEMAIL,target, "E-Mail   - routing@");
}



// fucking basic just to make JOIN work
void CommandHandler::handle_who(Client &target)
{
	get_replies(352, target);
	get_replies(315, target);
}
*/
//  fucking basic just to make JOIN work


// =============== Handle kick and invite 


