//
// Created by Louis-gabriel Laplante on 8/8/22.
//

/*
 * File to work on command parsing.
 * Commands are in plaintext, making it super duper easy to read. Parsing will work a bit like tokenizing
 */

#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>
#include <unordered_map>
#include <vector>

std::string commands[] = {
		"ADMIN",
		"AWAY",
		"CNOTICE",
		"CPRIVMSG",
		"CONNECT",
		"DIE",
		"ENCAP",
		"ERROR",
		"HELP",
		"INFO",
		"INVITE",
		"ISON",
		"JOIN",
		"KICK",
		"KNOCK",
		"LINKS",
		"LIST",
		"LUSERS",
		"MODE",
		"MOTD",
		"NAMES",
		"NAMESX",
		"NICK",
		"NOTICE",
		"OPER",
		"PART",
		"PASS",
		"PING",
		"PONG",
		"PRIVMSG",
		"QUIT",
		"REHASH",
		"RULES",
		"SERVER",
		"SERVICE",
		"SERVLIST",
		"SQUERY",
		"SQUIT",
		"SETNAME",
		"SILENCE",
		"STATS",
		"SUMMON",
		"TIME",
		"TOPIC",
		"TRACE",
		"UHNAMES",
		"USER",
		"USERHOST",
		"USERIP",
		"USERS",
		"VERSION",
		"WALLOPS",
		"WATCH",
		"WHO",
		"WHOIS",
		"WHOWAS"
};

std::string _ignored[] = {
		"CAP",
		"CNOTICE",
		"CPRIVMSG",
		"CONNECT",
		"ENCAP",
		"ERROR",
		"HELP",
		"INFO",
		"ISON",
		"KNOCK",
		"LINKS",
		"LUSERS",
		"NAMESX",
		"PONG",
		"REHASH",
		"RULES",
		"SERVER",
		"SERVICE",
		"SERVLIST",
		"SQUERY",
		"SQUIT",
		"SETNAME",
		"SILENCE",
		"STATS",
		"SUMMON",
		"TRACE",
		"UHNAMES",
		"USERHOST",
		"USERIP",
		"USERS",
		"WALLOPS",
		"WATCH",
		"WHOIS",
		"WHOWAS"
};

std::string _available[] = {
		"ADMIN", //return information about server admins of the current server
		"AWAY", //set current status to away(afk). without a parameter, the status is removed or inactive
		"DIE", //shutdown server
		"INVITE", //invite somebody to channel. channel doesnt need to exist, but if it does, only its members should be able to invite
		"JOIN", //join a channel
		"KICK", //kick user from channel
		"KILL", //kill client connection from entire server rather than channel
		"LIST", //list available channels
		"MODE", //set channel mode, or user modes
		"MOTD", //return server message of the day
		"NAMES",
		"NICK", //set account nickname on server
		"NOTICE",
		"OPER", //allows to login as Operator (channel ?? server ??)
		"PART",
		"PASS", //password login
		"PING", //ping server for timestamp
		"PRIVMSG", //send private message to user
		"QUIT", //end connection ?
		"TIME", //display time
		"TOPIC", //display channel topic
		"USER", //userrname login
		"VERSION", //display server version
		"WHO" //display list of users on a channel. @ users are channel ops, * users are irc ops
};

//thank you stack overflow
std::vector<std::string> split_string(const std::string& str,
									  const std::string& delimiter)
{
	std::vector<std::string> strings;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::string::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + delimiter.size();
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}

class Command {
private:
	std::string _command;

public:
};

void parse_command(char *buff)
{
	//std::unordered_map<std::string, int> irc_commands; //int as placeholder for function pointer
	std::vector<std::string> block = split_string(std::string(buff), "\n"); //so each line in here represents a command that we can then parse

	for (std::vector<std::string>::iterator it = block.begin(); it != block.end(); it++) { //should use iterators maybe ? i like the array style tho :(
		std::string input = *it;
		std::cout << input << std::endl;
		for (int i = 0; i < 24; i++) {
			if (input.find(_available[i], 0) != std::string::npos) {
				std::cout << "command: " << _available[i] << std::endl;
				std::cout << "parameters: " << input.substr(0 + _available[i].length(), input.length()) << std::endl;
				break;
			}

		}
	}
}