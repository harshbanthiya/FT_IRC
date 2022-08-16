#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include <iostream>
# include <string>
# define MAX_PARAMS 15

class Command
{

	public:

		Command();
		Command(std::string command, int num);
		Command( Command const &src);
		~Command();

		std::string 	get_command();
		void 			set_command(std::string cmd);

		int 			get_num_params();
		void 			set_num_params(int n);

		std::string 	get_param(int i);
		void 			set_param(int n, std::string val);
		Command &		operator=( Command const & rhs );

	private:
		std::string    	command;
    	int     		num_params;
    	std::string 	params[MAX_PARAMS];

};

#endif