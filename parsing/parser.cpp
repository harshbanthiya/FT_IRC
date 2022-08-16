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

void parse_command(char *buff)
{
	//std::unordered_map<std::string, int>() irc_commands; //int as placeholder for function pointer

	std::string input = std::string(buff);
	std::cout << input << std::endl;
	for (int i = 0;i < 56;i++)
	{
		if (input.find(commands[i], 0) != std::string::npos) {
			std::cout << "command: " << commands[i] << std::endl;
			std::cout << "parameters: " << input.substr(0, commands[i].length()) << std::endl;
			break;
		}
	}

}