/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:55:13 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/06 15:51:30 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdexcept>
#include <cerrno>
#include <unistd.h>
#include <vector>
# include <poll.h> 
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstdio>
#include <fstream>
#include <map>

#include "Client.hpp"
#include "Channel.hpp"
#include "Command_Handler.hpp"
#include "replies.h"

#define BACKLOG_CONNECTIONS 10
#define UMODES std::string("oiws")
#define CMODES std::string("obtkmlvsn")
#define MOTD_PATH "welcome_screen.txt"
#define SERV_NAME std::string("MyIRC")


class Server
{

		public:

			Server(std::string port, std::string paswd);
			~Server();

			void 										init();
			void 										execute();
			bool 										check_pass(std::string &pass);
			bool 										user_exists(std::string name);


			// Getters 
			std::vector<Client *> const 				&get_all_clients();
			Client const								&get_client(std::string nickname) const;
			std::string 								getcreatedTime();
			Channel										&get_channel(std::string channelName);
			const std::map<std::string, Channel> 		&get_channel_list() const ;
			std::string 								getPasswrd();
			CommandHandler								getHandler() const;
			std::vector<std::string> const				&get_motd() const;
			
			void 										disconnect_client(std::string nick);
			void 										disconnect_client(int index);

			void										send_msg(std::string& msg, Client const &target) const;
			int											send_msg(std::string& msg, std::string target) const;
			int 										send_msg(std::string& msg, std::string target, Client const &owner);
			void 										send_to_all(std::string msg, Client &owner);

			void										create_channel(std::string ch_name);
			bool 										add_channel(Channel ch);
			bool										check_channel(std::string target) const;
			void										remove_channel(std::string name);
	

		private:
		
			std::string									port;
			int 										sock_fd;
			std::string 								createdTime;
			std::vector<struct pollfd> 					pfds;
			std::string 								passwrd;
			std::map<std::string, Channel>				list_of_all_channels;
			std::vector<Client*> 						list_of_all_clients;
			CommandHandler 								handler;
			std::vector<std::string>					motd; 
			
			
			void 										load_motd(const char *file);
			void 										accept_client();
			void 										add_fd(int new_fd);
			void 										add_client();
			void										exec_command(Client &executor);
		
		
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