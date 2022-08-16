#include "Command.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Command::Command()
{
}

Command::Command( const Command & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Command::~Command()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Command &				Command::operator=( Command const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Command const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */