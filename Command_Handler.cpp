#include "Command_handler.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Command_handler::Command_handler()
{
}

Command_handler::Command_handler( const Command_handler & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Command_handler::~Command_handler()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Command_handler &				Command_handler::operator=( Command_handler const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Command_handler const & i )
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