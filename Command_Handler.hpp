/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_Handler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 23:58:30 by sheeed            #+#    #+#             */
/*   Updated: 2022/09/06 16:28:50 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
# define COMMAND_HANDLER_HPP

# include <string>
# include <list>
# include <map>

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
		std::list<std::string> 	parameters;
		Server 					 &serv;
		std::map<std::string, void(CommandHandler::*)(Client&)> handlers;	

		void 	parse_cmd(std::string cmd_line);
		void 	handle_pass(Client &owner);
		void 	handle_nick(Client &owner);
		void 	handle_user(Client &owner);
		void 	handle_motd(Client &owner);
		void 	handle_ping(Client &owner);
		void 	handle_join(Client &owner);
		void 	handle_privmsg(Client &owner);
		void 	handle_kick(Client &owner);
		void 	handle_mode(Client &owner);
		void 	welcomescreen(Client &owner);
		void 	handle_invite(Client &owner);
		void 	handle_time(Client &owner);
		void 	handle_quit(Client &owner);
		//void 	handle_who(Client &owner);
		//void 	handle_admin(Client &owner);
	};

#endif