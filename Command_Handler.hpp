/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_Handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 23:58:30 by sheeed            #+#    #+#             */
/*   Updated: 2022/08/24 14:28:40 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP


# include <string>
# include <map>
# include <list>

# include "Client.hpp"


	class Server;

	class CommandHandler
	{


		public:
			CommandHandler(Server &server);
			void handle(std::string cmd_line, Client& owner);
			void get_replies(int  code, Client const &owner, std::string extra = "")const;

		private:
			std::string    			command;
			std::vector<std::string> parameters;
			Server 					 &serv;
			std::map<std::string, void(CommandHandler::*)(Client&)> handlers;	

			void 	parse_cmd(std::string cmd_line);
			void 	handle_pass(Client &owner);
			void 	handle_user(Client &owner);
			void 	handle_nick(Client &owner);
			void 	handle_ping(Client &owner);
			void 	handle_privmsg(Client &owner);
			void 	print_welcome(Client &owner);
			void 	welcomescreen(Client &owner);
	};

#endif