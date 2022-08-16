#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>

class Command
{

	public:

		Command();
		Command( Command const & src );
		~Command();

		Command &		operator=( Command const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, Command const & i );

#endif /* ********************************************************* COMMAND_H */