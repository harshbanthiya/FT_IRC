/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 23:58:30 by sheeed            #+#    #+#             */
/*   Updated: 2022/08/17 16:38:06 by hbanthiy         ###   ########.fr       */
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

namespace irc
{


	class Client;
	class Server;
	class Command
	{


		public:

			Command(Client *client, Server *server, std::string message);
			//Command(std::string command, int num);
		
			std::string 	getprefix();
			std::vector<std::string> getParameters();
			std::string 	getTrailer();
			std::string 	getQuery();
			bool 			getStop();
			Client 			&getClient();
			Server 			&getServer();
			//void 			set_command(std::string cmd);

			//int 			get_num_params();
			//void 			set_num_params(int n);
			void 			setStop();
			void 			reply(Client &client, unsigned short code, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "", std::string arg5 = "", std::string arg6 = "", std::string arg7 = "");
			void 			reply(unsigned short code, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "", std::string arg5 = "", std::string arg6 = "", std::string arg7 = "");

			//std::string 	get_param(int i);
			//void 			set_param(int n, std::string val);

		private:
			Client 			*client;
			Server 			*server;
			std::string    	prefix;
			std::vector<std::string> parameters;
			std::string trailer;
			std::string query;
			bool stop;
			// std::string getReplies(); functions reply 

	};
}
#endif