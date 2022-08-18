/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 19:55:13 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/17 20:45:56 by olabrecq         ###   ########.fr       */
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

			Server(int port, std::string paswd);
			Server( Server const & src );
			~Server();

			Server &		operator=( Server const & rhs );

			void 			init();
			void 			execute();

		// Getters 
			std::vector<Client *> 		get_all_clients();
			Client* 			  		get_client(std::string &nickname);
			void 						disconnect_client(Client &client);
			std::string 				getUpTime();
			void 						sendPing();


		// Setters 

		private:
		
		int						port; 
		unsigned int 			total_client_count;
		int 					fd;
		time_t					last_ping;
		std::string 			upTime;
		std::vector<pollfd> 	pfds;
		struct sockaddr_in 		addr_info;
		std::string 			passwrd;
		std::string 			server_ip_addr;
		std::map<int, Client*> 	list_of_all_clients;
		void 					acceptClient();
		
		// channel list 
		// Operator list
		// command map 
		// reply map 

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