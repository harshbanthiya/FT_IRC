//
// Created by Louis-gabriel Laplante on 8/17/22.
//
#include "Client.hpp"
#include "Server.hpp"
#include "Command.hpp"

bool check_if_username_exists(irc::Server server, std::string nickname)
{
	std::vector<irc::Client *> clients = server.get_all_clients();
	for(std::vector<irc::Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		irc::Client &client = reinterpret_cast<irc::Client &>(*it);
		if (client.get_nickname() == nickname)
			return true;
	}
	return false;
}

void NICK(irc::Command *command)
{
	irc::Client &client = command->getClient();
	if (!command->getParameters().size())
	{
		return command->reply(431); //ERR_NONICKNAMEGIVEN
	}
	if (check_if_username_exists(command->getServer(), command->getParameters()[0]) == false) { //nickname is not in use
		if (isdigit(command->getParameters()[0][0]) != 0 || command->getParameters()[0].find_first_of(".?! ") != std::string::npos || command->getParameters()[0][0] == '-') //if the first char is a number, or whole thing contains banned chars
		{
			return command->reply(432); //ERR_ERRONEUSNICKNAME
		}
		if (client.get_status() == irc::ONLINE)
			command->reply(client, 0, client.get_nickname(), command->getprefix(), command->getParameters()[0]); //if online, send back a reply
		return client.set_nickname(command->getParameters()[0]); //0 because only 1 parameter, also dont forget to remove CRLF here
	}
	else
	{
		return command->reply(433); //ERR_NICKNAMEINUSE
	}
}

void USER(irc::Command *command)
{
	irc::Client &client = command->getClient();
	if (command->getParameters().size() < 4)
	{
		return command->reply(461); //ERR_NEEDMOREPARAMS
	}
	if(client.get_registered() == true)
	{
		return command->reply(462); //ERR_ALREADYREGISTRED
	}
	client.set_username(command->getParameters()[0]);
	//mode is missing
	client.set_realname(command->getParameters()[3]);
	client.set_registered(true);
	client.set_status(irc::ONLINE);
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