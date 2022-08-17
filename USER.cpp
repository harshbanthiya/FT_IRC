#include "Command.hpp"
#include "Client.hpp"

void USER(irc::Command *command)
{
	if (command->getParameters().size() < 3)
		return command->reply(461, command->getprefix());
	if (command->getClient().get_status() != irc::REGISTER)
		return command->reply(462);

	command->getClient().set_username(command->getParameters()[0]);
	command->getClient().set_realname(command->getTrailer());
}
