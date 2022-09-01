/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_Handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 13:37:38 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/01 11:09:59 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_Handler.hpp"
#include "Server.hpp"
#include <fstream>
std::ifstream infile("welcome_screen.txt");

CommandHandler::CommandHandler(Server &_server): serv(_server)
{
	this->handlers["PASS"] = &CommandHandler::handle_pass;
	this->handlers["NICK"] = &CommandHandler::handle_nick;
	this->handlers["PING"] = &CommandHandler::handle_ping;
	this->handlers["USER"] = &CommandHandler::handle_user;
	this->handlers["ADMIN"] = &CommandHandler::handle_admin;
	this->handlers["TIME"] = &CommandHandler::handle_time;
	this->handlers["PRIVMSG"] = &CommandHandler::handle_privmsg;
	this->handlers["JOIN"] = &CommandHandler::handle_join;
	this->handlers["WHO"] = &CommandHandler::handle_who;
	this->handlers["MODE"] = &CommandHandler::handle_mode;
	// this->handlers["DIE"] = &CommandHandler::handle_user;
	/*
			LUSERS
		PART
		AWAY
			QUIT
		KICK
		NAMES
		INVITE
		LIST
		TOPIC
	*/
}

void 	CommandHandler::parse_cmd(std::string cmd_line)
{
	std::cout << cmd_line << std::endl;
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
			cmd_line.erase();
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
		return get_replies(451, owner, command);
	if (handlers.find(command) == handlers.end())
		get_replies(421, owner, command);
	else 
		(*this.*(this->handlers[this->command]))(owner);
} 

void 	CommandHandler::handle_user(Client &owner)
{
	if (parameters.size() < 4)
		return get_replies(ERR_NEEDMOREPARAMS, owner, command); //ERR_NEEDMOREPARAMS
	if(owner.is_registered())
		return get_replies(ERR_ALREADYREGISTERED, owner, command); //ERR_ALREADYREGISTRED
	std::string username = parameters.front();
	if (username.empty())
		return get_replies(ERR_NEEDMOREPARAMS, owner, command);
	std::string realname = parameters.back();
	//mode is missing
	owner.set_username(username);
	owner.set_realname(realname);
	if (!owner.get_nickname().empty())
		print_welcome(owner);
}

void	CommandHandler::handle_pass(Client &owner)
{

	if (!parameters.size() || parameters.front() == "")
		return get_replies(ERR_NEEDMOREPARAMS, owner, command); //ERR_NEEDMOREPARAMS
	if(owner.is_registered())
		return get_replies(ERR_ALREADYREGISTERED, owner); //ERR_ALREADYREGISTRED
	if (serv.checkPass(this->parameters.front()))
		owner.set_passed();
	else 
		get_replies(ERR_PASSWDMISMATCH, owner);
}

void 	CommandHandler::handle_ping(Client &owner)
{
	if (!parameters.size() || parameters.front() == "")
		return get_replies(ERR_NEEDMOREPARAMS, owner, command);
	std::string msg = ":" + std::string("MyIRC") + " PONG " + ":" + parameters.front() + END_DELIM;
	this->serv.send_msg(msg, owner);
}

void	CommandHandler::handle_nick(Client &owner)
{
	if (!parameters.size() || parameters.front() == "")
		return get_replies(ERR_NONICKNAMEGIVEN, owner, command);
	
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
		print_welcome(owner);
	
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
 		// if (curr_target[0] == '#')
			//rv = this->serv.send_msg(msg, curr_target, owner); // put all the logic in the overloaded function
 		// else
 			rv = this->serv.send_msg(msg, curr_target);
 		if (rv == ERR_NOSUCHNICK)
 			get_replies(rv, owner, curr_target);
 		targets.erase(0, (pos != -1) ? pos + 1 : pos);
 	}
 }

void 	CommandHandler::get_replies(int code, Client const &owner, std::string extra) const
{
	std::string msg = ":" + std::string("MyIRC") + " ";
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
		case RPL_YOURHOST: 
			msg += ":Your host is " + std::string("MyIRC") + ", running version IRC1.0";
			break;
		case RPL_CREATED: 
			msg += ":This server was created " + extra;
			break;
		case PRINT_SCREEN:
			msg += "-: " + extra;
			break;
		case ERR_PASSWDMISMATCH:
			msg += ":Password incorrect";
			break;
		case ERR_NONICKNAMEGIVEN:
			msg += ":No nickname given";
			break;
		case ERR_NICKNAMEINUSE:
			msg += extra + " :Nickname is already in use"; 
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
		case RPL_AWAY:
			msg += extra;
		case RPL_WHOREPLY:
			msg += extra;// + " :<hopcount> <real name>";
			break;
		case RPL_ENDOFWHO:
			msg += extra + " :End of /WHO list";
			break;
		case RPL_CHANNELMODEIS: 
			msg += extra;
			break;
		case RPL_CREATIONTIME:
			msg += extra;
			break;
		case RPL_ENDOFBANLIST:
			msg += extra + " :End of channel ban list";
			break;
		case RPL_NAMREPLY:
			msg += extra;
			break;
		case RPL_ENDOFNAMES:
			msg += extra + " :End of /NAMES list";
			break;
		case ERR_NORECIPIENT:
			msg += ":No recepient given ( " + extra + ")";
			break;
		case ERR_NOTEXTTOSEND:
			msg += ":No text to send";
			break;
		case ERR_NOSUCHNICK:
			msg += extra + " :No such nick";
			break;
		case ERR_NOSUCHCHANNEL:
			msg += extra + " :No such channel";
			break;
		case ERR_UNKNOWNMODE:
			msg += extra + " :is unknown mode char to me";
			break;
		case ERR_CHANOPRIVSNEEDED:
			msg += extra + " :You're not channel operator";
			break;
		
    }
	msg += END_DELIM;
	serv.send_msg(msg, owner);
}

void CommandHandler::welcomescreen(Client &target)
{
	std::string line;
	while (std::getline(infile, line))
	{
		get_replies(PRINT_SCREEN, target, line);
	}
	infile.close();
}

void 	CommandHandler::print_welcome(Client &target)
{
	target.set_registered();
	get_replies(RPL_WELCOME, target);
	get_replies(RPL_YOURHOST, target);
	get_replies(RPL_CREATED, target, this->serv.getcreatedTime());
	welcomescreen(target);
}

void CommandHandler::handle_time(Client &target)
{
	std::time_t result = std::time(nullptr);
    
	std::string msg = std::string(std::asctime(std::localtime(&result))); 
	get_replies(RPL_TIME, target, msg);
}

void CommandHandler::handle_admin(Client &target)
{
	get_replies(RPL_ADMINME, target, " :Administrative info");
	get_replies(RPL_ADMINLOC1, target, "Name     - The Routing Team");
	get_replies(RPL_ADMINLOC2, target, "Nickname - #Routing");
	get_replies(RPL_ADMINEMAIL,target, "E-Mail   - routing@");
}

void CommandHandler::handle_join(Client &owner)
{

	if (!this->parameters.size())
		return get_replies(ERR_NEEDMOREPARAMS, owner);
	int 	pos;
	std::list<std::string> names;
	std::list<std::string>::iterator it;

	while (parameters.front() != "")
	{
		pos = parameters.front().find(",");
		names.push_back(parameters.front().substr(0, pos));
		parameters.front().erase(0, (pos != -1) ? pos + 1 : pos);
	}
	parameters.pop_front();
	if (names.front()[0] != '#')
		return get_replies(ERR_NOSUCHCHANNEL, owner, names.front());
	for (it = names.begin(); it != names.end(); it++)//while (!names.empty()) -> infini loop bad idea
	{
		if (!serv.check_channel(names.front())) 
		{
			printf("yeeehhaaa\n");
			Channel new_chan(names.front(), serv);
			// if (serv.add_channel(new_chan)); implement
		}

	}

}
	

 
// fucking basic just to make JOIN work
void CommandHandler::handle_who(Client &target)
{
//  352 olabrecq #jani ~olabrecq freenode-g2q.k15.7sskld.IP *.freenode.net olabrecq H@ 0 Olivier Labrecque-lacasse
//  315 olabrecq #jani End of /WHO list.
	std::list<std::string>::iterator it;
	for(it = parameters.begin(); it != parameters.end(); it++)
		printf("param = %hhd\n", *it->c_str());
	std::string msg = parameters.front() + " ~" + target.get_nickname() + " " + target.get_hostname() + "*MyIRC " + target.get_nickname() + " H@ 0 " + target.get_realname();
	get_replies(RPL_WHOREPLY, target, msg);//352
	msg = parameters.front();
	get_replies(RPL_ENDOFWHO, target, msg);
	// this->serv.send_msg(msg, target);
}

//  fucking basic just to make JOIN work
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
			get_replies(RPL_CHANNELMODEIS, owner, target + "+" + serv.get_channel(target).get_modes());
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

}
