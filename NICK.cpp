#include "Command.hpp"

void NICK(irc::Command *command)
{
    if (!command->getParameters().size())
		return command->reply(001);
}