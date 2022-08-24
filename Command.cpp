/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 13:37:38 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/23 19:52:47 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Server.hpp"



// This function needs love 
irc::CommandHandler::CommandHandler(Server &_server) : serv(_server)
{
	this->handlers["PASS"] = &CommandHandler::handle_pass;
	this->handlers["NICK"] = &CommandHandler::handle_nick;
	this->handlers["PING"] = &CommandHandler::handle_ping;
}

void 	irc::CommandHandler::parse_cmd(std::string cmd_line)
{
	if (cmd_line.empty())
		return ;
	int pos = cmd_line.find(" ");
	command = toUpper(cmd_line.substr(0, pos));
	cmd_line.erase(0, (pos != -1) ? pos + 1 : pos);
	parameters.clear();
	while (!cmd_line.empty())
	{
		if (cmd_line[0] == ":")
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

void irc::CommandHandler::handle(std::string cmd_line, Client &owner)
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

/*
void irc::Command::reply(Client &client, unsigned short code, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7)
{
	std::stringstream sscode;
	sscode << code;
	std::string scode = sscode.str();
	while (scode.length() < 3)
		scode = "00" + scode;

	client.sendTo(client, scode + " " + getReplies(code, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
}
void irc::Command::reply(unsigned short code, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7) { reply(*client, code, arg1, arg2, arg3, arg4, arg5, arg6, arg7); }
*/
void 	irc::CommandHandler::handle_user(Client &owner)
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
	if (!owner.get_nickname().empty())
		print_welcome(owner);
}

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
