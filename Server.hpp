/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:55:13 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/28 19:24:47 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <unistd.h>
#include <vector>
# include <poll.h> 
#include <arpa/inet.h>
#include <fcntl.h>
# include <iostream>
# include <exception>
# include <cstdio>
#include <fstream>
#include <netinet/in.h>
#include <map>

#include "Client.hpp"
#include "Channel.hpp"
#include "Command_Handler.hpp"
#include "replies.h"
#include <ctime>

class Server
{

		public:

			Server(std::string port, std::string paswd);
			~Server();
			void 			init();
			void 			execute();
			bool 			checkPass(std::string &pass);
			bool 			user_exists(std::string name);


		// Getters 
			std::vector<Client *> const &get_all_clients();
			Client const				&get_client(std::string nickname) const;
			void 						disconnect_client(std::string nick);
			void 						disconnect_client(int index);
			std::string 				getcreatedTime();
			void 						sendPing();
			std::string 				getPasswrd();
			CommandHandler				getHandler() const;

			void						send_msg(std::string& msg, Client const &target) const;
			int							send_msg(std::string& msg, std::string target) const;
			bool						checkChannel(std::string target) const;
			

		// Setters 

		private:
		
			std::string					port; 
			int 						sock_fd;
			time_t						last_ping;
			std::string 				createdTime;
			std::vector<struct pollfd> 	pfds;
			std::string 				passwrd;
			std::vector<Client *> 		list_of_all_clients;
			void 						acceptClient();
			CommandHandler 				_handler; 
			void 						add_fd(int new_fd);
			void 						add_client();
			void						exec_command(Client &executor);

			std::map<std::string, Channel *> list_of_all_channel;
		
		
		// Exceptions 
		class SocketFailException : public std::exception
		{
			public: virtual const char* what() const throw();
		};

		class SetsockoptFailException : public std::exception
		{
			public: virtual const char* what() const throw();
		};
		
		class BindFailException : public std::exception
		{
			public: virtual const char* what() const throw();
		};
		
		class ListenFailException : public std::exception
		{
			public: virtual const char* what() const throw();
		};
		class FcntlFailException : public std::exception
		{
			public: virtual const char* what() const throw();
		};
};

#endif