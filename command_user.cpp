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
		if (isdigit(command->getParameters()[0][0]) || command->getParameters()[0].find_first_of(".?! ") || command->getParameters()[0][0] == '-') //if the first char is a number, or whole thing contains banned chars
		{
			return command->reply(432); //ERR_ERRONEUSNICKNAME
		}
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
		return command->reply(464); //ERR_PASSWDMISMATCH
	}
	//password is correct, send nothing
	return;
}

void ADMIN(irc::Command *command) 
{
    if (!command->getParameters().size()) 
	{
        command->reply(256); //     RPL_ADMINME
        command->reply(257); //     RPL_ADMINLOC1
        command->reply(258); //     RPL_ADMINLOC2
        command->reply(259);//      RPL_ADMINEMAIL
        return;
    }
}

// void DIE(irc::Command *command)
// {
	
// }

void QUIT(irc::Command *command)
{
	return (command->reply(69));
}