/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_Handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sheeed <sheeed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 13:37:38 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/25 19:00:40 by sheeed           ###   ########.fr       */
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
	this->handlers["PRIVMSG"] = &CommandHandler::handle_privmsg;
	/*
		LUSERS
		JOIN
		PART
		PRIVMSG
		AWAY
		QUIT
		WHO
		KICK
		MODE
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
		//if (curr_target[0] == '#')
		//	rv = this->serv.send_msg(msg, curr_target, owner);

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
		case ERR_NORECIPIENT:
			msg += ":No recepient given ( " + extra + ")";
			break;
		case ERR_NOTEXTTOSEND:
			msg += ":No text to send";
			break;
		case ERR_NOSUCHNICK:
			msg += extra + " :No such nick";
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