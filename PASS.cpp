#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"

void PASS(irc::Command *command)
{
	if (!command->getParameters().size())
		return command->reply(001);
	if (command->getClient().get_status() != irc::PASSWORD)
		return command->reply(001);
}
