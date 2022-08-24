#include "Command.hpp"
#include "Client.hpp"

std::string RPL_WELCOME(std::string prefix) { return ":Welcome to the Internet Relay Network " + prefix; }
std::string RPL_YOURHOST(std::string servername, std::string ver) { return ":Your host is " + servername + ", running version " + ver; }
std::string RPL_CREATED(std::string date) { return ":This server was created " + date; }
std::string RPL_MYINFO(std::string servername, std::string version, std::string umodes, std::string cmodes) { return servername + " " + version + " " + umodes + " " + cmodes; }
std::string RPL_BOUNCE(std::string server, std::string port) { return ":Try server " + server + ", port " + port; }
std::string ERR_NOORIGIN() { return ":No origin specified"; }
std::string ERR_NONICKNAMEGIVEN() { return ":No nickname given"; }
std::string ERR_ERRONEUSNICKNAME(std::string nick) { return nick + " :Erroneus nickname"; }
std::string ERR_NICKNAMEINUSE(std::string nick) { return nick + " :Nickname is already in use"; }
std::string RPL_PRINTWELCOME(std::string text) { return "- " + text; }
std::string irc::CommandHandler::getReplies(int code, Client &owner, std::string extra)
{
	std::string msg = ":" + "MyIRC" + " ";
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
	case 001:
		 msg += ":Welcome to the Internet Relay Network " ;
		 msg += owner.get_nickname() + "!" + owner.get_username() + "@" + owner.get_hostname();
	
    }
	msg += END_DELIM;
	serv.send_msg(msg, owner);
}