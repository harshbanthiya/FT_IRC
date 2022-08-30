/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:55:13 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/30 16:35:45 by olabrecq         ###   ########.fr       */
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
#include <list>
#include <unordered_map>

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
			int 						send_msg(std::string& msg, std::string target, Client const &owner); // Added for channel priv
			bool						check_channel(std::string target) const;
			Channel						&get_channel(std::string channelName);
			const std::vector<std::string, Channel> &get_channel_list() const ;

			void						create_channel(std::string ch_name);
		// Setters 
			// bool 						add_channel(Channel ch); // Implement later once channel class has all the getters and setters

		private:
		
			std::string					port;
			int 						sock_fd;
			time_t						last_ping;
			std::string 				createdTime;
			std::vector<struct pollfd> 	pfds;
			std::string 				passwrd;
			std::vector<Client *> 		list_of_all_clients;
			
			std::map<std::string, Channel>	list_of_all_channels;
			
			void 						acceptClient();
			CommandHandler 				_handler; 
			void 						add_fd(int new_fd);
			void 						add_client();
			void						exec_command(Client &executor);
		
		
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