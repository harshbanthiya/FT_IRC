//
// Created by Louis-gabriel Laplante on 8/17/22.
//

#include "Command.hpp"
#include "Utils.hpp"

#include <fstream>
std::ifstream infile("welcome_screen.txt");

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
		return command->reply(431); //ERR_NONICKNAMEGIVEN
	std::string nickname = command->getParameters()[0];
	
	// Separated 432 in 3 parts for clarity and better error handling 
	if (nickname.length() > 9)
		return (command->reply(432, nickname));
	size_t index = 0;
	if (!irc::isLetter(nickname[index]) && !irc::isSpecial(nickname[index]))
		return (command->reply(432, nickname));
	++index;
	for (; index < nickname.length(); ++index)
		if (!irc::isLetter(nickname[index]) && !irc::isSpecial(nickname[index]) && !irc::isDigit(nickname[index]) && nickname[index] != '-')
			return command->reply(432, nickname);
	
	// For 433 Nickinuse we will loop through list of all clients in Server class
	std::vector<irc::Client *>clients = command->getServer().get_all_clients();
	for (std::vector<irc::Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		if (nickname == (*it)->get_nickname())
			return (command->reply(433, nickname));
	
	// Else set nick name
	command->getClient().set_nickname(nickname);
	/*
	if (check_if_username_exists(command->getServer(), command->getParameters()[0]) == false) { //nickname is not in use
		if (isdigit(command->getParameters()[0][0]) != 0 || command->getParameters()[0].find_first_of(".?! ") != std::string::npos || command->getParameters()[0][0] == '-') //if the first char is a number, or whole thing contains banned chars
		{
			return command->reply(432); //ERR_ERRONEUSNICKNAME
		}
		if (client.get_status() == irc::ONLINE)
			command->reply(client, 0, client.get_nickname(), command->getprefix(), command->getParameters()[0]); //if online, send back a reply
		return client.set_nickname(command->getParameters()[0]); //0 because only 1 parameter, also dont forget to remove CRLF here
	}
	*/
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

void PRINT_WELCOME(irc::Command *command)
{
	std::string line;
	while (std::getline(infile, line))
	{
		command->reply(372, line);
	}
	infile.close();
}

void ADMIN(irc::Command *command) 
{
    if (command->getParameters().size() == 0) 
	{
        command->reply(256); //     RPL_ADMINME
        command->reply(257); //     RPL_ADMINLOC1
        command->reply(258); //     RPL_ADMINLOC2
        command->reply(259);//      RPL_ADMINEMAIL
        return;
    }
}

void TIME(irc::Command *command)
{
	if (command->getParameters().size() == 0)
		command->reply(391);
}

void DIE(irc::Command *command)
{
	if (command->getParameters().size() == 0)
		command->reply(391);
}