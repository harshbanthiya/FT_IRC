#include "Command.hpp"
#include "Client.hpp"

std::string RPL_WELCOME(std::string prefix) { return ":Welcome to the Internet Relay Network " + prefix; }
std::string RPL_YOURHOST(std::string servername, std::string ver) { return ":Your host is " + servername + ", running version " + ver; }
std::string RPL_CREATED(std::string date) { return ":This server was created " + date; }
std::string RPL_MYINFO(std::string servername, std::string version, std::string umodes, std::string cmodes) { return servername + " " + version + " " + umodes + " " + cmodes; }
std::string RPL_BOUNCE(std::string server, std::string port) { return ":Try server " + server + ", port " + port; }

std::string irc::Command::getReplies(unsigned short code, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7)
{
	std::string target;
	if (client->get_status() == PASSWORD || client->get_status() == REGISTER)
		target = "*";
	else
		target = client->get_nickname();
	target += " ";

	switch (code)
	{
	case 001:
		return target + RPL_WELCOME(arg1);
	case 002:
		return target + RPL_YOURHOST(arg1, arg2);
	case 003:
		return target + RPL_CREATED(arg1);
	case 004:
		return target + RPL_MYINFO(arg1, arg2, arg3, arg4);
	case 005:
		return target + RPL_BOUNCE(arg1, arg2);
    default: 
       return std::string();
    }
}