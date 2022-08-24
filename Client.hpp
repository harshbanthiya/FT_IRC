/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 18:53:47 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/23 19:26:29 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <netinet/in.h>
# include <poll.h>
# include "Server.hpp"
# include "Command.hpp"
# include <ctime>

namespace irc
{


	class Command; 
	class Server ;

	enum ClientStatus
	{
			PASSWORD,
			REGISTER,
			ONLINE ,
			DELETE 
		
	};
	class Client
	{
		friend class Server;
		

		public:

			// Constructors 
			Client(int fd, std::string host);
			~Client();
			
			bool				operator==(Client const & other) const;
			bool				operator==(std::string const & other) const;

			std::string& 	buffer();
			bool 			is_passed()const;
			bool 			is_registered()const;
			
			void 			set_passed();
			void 			set_registered();

			void 			sendTo(Client &toclient, std::string message);
			// Getters 
			
			std::string	 		get_hostname(void) const;
			std::string	 		get_nickname(void) const;
			std::string	 		get_username(void) const;
			std::string	 		get_realname(void) const;
			std::string	 		get_client_ip_addr(void) const;
			bool				get_nick_bool(void);
			bool				get_user_bool(void);
			bool 				get_registered(void);
			std::string& 		get_buffer(int std_no);
			ClientStatus 		get_status();
			int 				getSocket();
			std::string 		getprefix();
			time_t				getLastPing();
			// Setters 
			void 				set_status(ClientStatus status);
			void 				set_nickname(std::string nick_n);
			void 				set_username(std::string user_n);
			void 				set_hostname(std::string host_n);
			void 				set_realname(std::string real_n);
			void				set_client_ip_addr(std::string ip_add);
			void 				set_nick_bool(bool val);
			void 				set_user_bool(bool val);
			void 				set_registered(bool val);
		
		private:
		
			int 			socket_fd;
			std::string 	hostname;
			bool 			pass_set;
			bool			registered;
			std::string 	nickname;
			std::string 	username;
			std::string 	realname;
			std::string 	buffer;

			time_t 			last_ping;
			std::string		toUpper(std::string const & str);
	};
}
#endif	