/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 23:58:30 by sheeed            #+#    #+#             */
/*   Updated: 2022/08/23 19:55:51 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include <string>
# define MAX_PARAMS 15
# include "Server.hpp"
# include "Client.hpp"
# include <vector>
# include <map>
# include <list>

namespace irc
{


	class Client;
	class Server;
	class CommandHandler
	{


		public:

			CommandHandler(Server *server);
			void handle(std::string cmd_line, Client& owner);

		private:
			std::string    			command;
			std::vector<std::string> parameters;
			Server 					 &serv;
			std::map<std::string, void(CommandHandler::*)(Client&)> handlers;	
			void get_replies(int  code, Client const &owner, std::string extra = "")const;

			void 	parse_cmd(std::string cmd_line);
			void 	handle_pass(Client &owner);
			void 	handle_user(Client &owner);
			void 	handle_nick(Client &owner);
			void 	handle_ping(Client &owner);

	};
}
#endif