#include "Command_Handler.hpp"

Command::Command() : command{NULL}, num_params{0}{}

Command::Command(std::string cmd, int num) : command{cmd}, num_params{num}{}

Command::Command( const Command &rhs ){*this = rhs;}

Command::~Command(){}

std::string 	Command::get_command()
{
	return (command);
}
void 			Command::set_command(std::string cmd)
{
		command = cmd;
}

int 			Command::get_num_params()
{
	return (num_params);
}
void 			Command::set_num_params(int n)
{
		num_params = n;
}

std::string 	Command::get_param(int i)
{
	return (params[i]);
}
void 			Command::set_param(int n, std::string val)
{
	if (n >= 0 && n < MAX_PARAMS)
		params[n] = val;
	
}


Command &				Command::operator=( Command const &rhs)
{
	if ( this == &rhs )
		return(*this);
	
	this->command = rhs.command;
	this->num_params = rhs.num_params;
	for(int i = 0; i < MAX_PARAMS; i++)
		this->params[i] = rhs.params[i];
	return *this;
}