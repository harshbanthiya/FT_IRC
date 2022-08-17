//
// Created by Louis-gabriel Laplante on 8/17/22.
//
#include "../Client.hpp"
#include "../Server.hpp"
#include "../Command_.hpp"

bool check_if_username_exists(Server *server)
{

}
void NICK(Command *command)
{
	Client *client;
	if (!command->getParameters().size())
	{
		return command->reply(431); //ERR_NONICKNAMEGIVEN
	}
	client = command->getClient();
	if (check_if_username_exists(command->getServer()) == false) { //nickname is not in use
		client->set_nickname(command->getParameters()[0]); //0 because only 1 parameter, also dont forget to remove CRLF here
	}
	else
	{
		return command->reply(433) //ERR_NICKNAMEINUSE
	}
}

