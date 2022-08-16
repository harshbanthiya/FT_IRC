#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include <iostream>
# include <string>

class Command_handler
{

	public:

		Command_handler();
		Command_handler( Command_handler const & src );
		~Command_handler();

		Command_handler &		operator=( Command_handler const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Command_handler const & i );

#endif /* ********************************************************* Command_handler_H */