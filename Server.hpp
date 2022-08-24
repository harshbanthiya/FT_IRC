/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:55:13 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/23 18:41:19 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <string>
# include <exception>
# include <poll.h> // Have a lot of common includes make a master header later 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include "Client.hpp"
#include <ctime>

#ifndef DEBUG
	#define DEBUG 0
#endif
#define PORT 	6667
namespace irc
{


	class Client;
	class Server
	{

		public:

			Server(std::string port, std::string paswd);
			~Server();
			void 			init();
			void 			execute();

		// Getters 
			std::vector<Client *> 		get_all_clients();
			Client& 			  		get_client(std::string nickname);
			void 						disconnect_client(std::string nick);
			void 						disconnect_client(int index);
			std::string 				getcreatedTime();
			void 						sendPing();
			std::string 				getPasswrd();
			void						send_msg(std::string& msg, Client &target);
			bool 						user_exists(std::string name);

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
		CommandHandler 				handler; 
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
}
#endif