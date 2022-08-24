/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_Handler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 13:37:38 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/24 13:29:33 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command_Handler.hpp"
#include "Server.hpp"

// This function needs love 
CommandHandler::CommandHandler(Server &_server): serv(_server)
{
	//this->handlers["PASS"] = &CommandHandler::handle_pass;
	//this->handlers["NICK"] = &CommandHandler::handle_nick;
	//this->handlers["PING"] = &CommandHandler::handle_ping;
	this->handlers["USER"] = &CommandHandler::handle_user;
}

void 	CommandHandler::parse_cmd(std::string cmd_line)
{
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
		return get_replies(461, owner, command); //ERR_NEEDMOREPARAMS
	if(owner.is_registered())
		return get_replies(462, owner, command); //ERR_ALREADYREGISTRED
	std::string username = parameters.front();
	if (username.empty())
		return get_replies(461, owner, command);
	std::string realname = parameters.back();
	//mode is missing
	owner.set_username(username);
	owner.set_realname(realname);
	//owner.set_registered(true);
	//if (!owner.get_nickname().empty())
		//print_welcome(owner);
}
/*
void PASS(irc::Command *command)
{
	irc::Server &server = command->getServer();
	irc::Client &client = command->getClient();
	if (command->getParameters().size() < 1)
		return command->reply(461); //ERR_NEEDMOREPARAMS
	if(client.get_registered() == true)
	{
		return command->reply(462); //ERR_ALREADYREGISTRED
	}
	if (server.getPasswrd() != command->getParameters()[0])
	{
		//command->getClient().set_status(irc::DELETE);
		return command->reply(464); //ERR_PASSWDMISMATCH

	}
	if (server.getPasswrd() == command->getParameters()[0])
		command->getClient().set_status(irc::REGISTER);
	//password is correct, send nothing
}

void PING(class irc::Command *command)
{
	if (command->getParameters().size() == 0)
		return command->reply(409);
	command->getClient().sendTo(command->getClient(), "PONG :" + command->getParameters()[0]);
}
*/


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
    }
	msg += END_DELIM;
	serv.send_msg(msg, owner);
}