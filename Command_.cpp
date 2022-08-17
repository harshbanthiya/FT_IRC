#include "Command_.hpp"
#include <sstream>

// This function needs love 
Command::Command(Client *_client, Server *_server, std::string message) : client(_client), server(_server), query(message)
{
	std::string delimiter(":");
	size_t position;
	if ((position = message.find(delimiter)) != std::string::npos)
	{
		std::string tmp = message.substr(0, position);
		message.erase(0, position + delimiter.length());
		trailer = message;
		message = tmp;
	}

	// parameters = split(message, " "); need to write a split
	command_name = *(parameters.begin());
	parameters.erase(parameters.begin());

	for (size_t index = 0; index < command_name.length(); ++index)
		command_name[index] = std::toupper(command_name[index]);
}

Client& Command::getClient() { return *client; }
Server& Command::getServer() { return *server; }

std::string Command::get_command() { return command_name; }
std::vector<std::string> Command::getParameters() { return parameters; }
std::string Command::getTrailer() { return trailer; }
std::string Command::getQuery() { return query; }

void Command::reply(Client &client, unsigned short code, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7)
{
	std::stringstream sscode;
	sscode << code;
	std::string scode = sscode.str();
	while (scode.length() < 3)
		scode = "0" + scode;

	client.sendTo(client, scode + " " ); // + getReplies(code, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
}
void Command::reply(unsigned short code, std::string arg1, std::string arg2, std::string arg3, std::string arg4, std::string arg5, std::string arg6, std::string arg7) { reply(*client, code, arg1, arg2, arg3, arg4, arg5, arg6, arg7); }

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