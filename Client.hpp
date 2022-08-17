/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 18:53:47 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/15 19:54:51 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <map>
# include "Command_.hpp"
# include <netinet/in.h>
# include <poll.h>
# include "Server.hpp"
# include "Command_.hpp"

class Command;
class Server ;

class Client
{
	enum ClientStatus
	{
		PASSWORD,
		REGISTER,
		ONLINE,
		DELETE
	};

	public:

		// Constructors 
		Client(int fd, sockaddr_in6 addr);
		Client( Client const &rhs);
		~Client();
		Client &		operator=( Client const & rhs );

		void			append_buffer(int std_n, const std::string& new_buff);
		void			clear_buffer(int std_n);
		void 			sendTo(Client &toclient, std::string message);
		// Getters
		
		std::string	 		get_nickname(void) const;
		std::string	 		get_username(void) const;
		std::string	 		get_hostname(void) const;
		std::string	 		get_realname(void) const;
		std::string	 		get_client_ip_addr(void) const;
		bool				get_nick_bool(void);
		bool				get_user_bool(void);
		bool 				get_registered(void);
		std::string& 		get_buffer(int std_no);
		ClientStatus 		get_status();
		int 				get_fd;
		std::string 		getprefix();

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
		Client(void);
		std::map<std::string, void (*) (Command *)>command_function;
		std::vector<Command *> commands;
		std::vector<std::string> waitingtoSend;
		std::string 	nickname;
		std::string 	username;
		std::string 	realname;
		std::string 	hostname;
		std::string 	client_ip_addr;
		sockaddr_in6	client_addr;
		std::string 	buffer[2]; // IN buffer 0 and out buffer is 1
		ClientStatus	status;
		int 			fd;
		struct sockaddr_in6 client_addr_info;
		pollfd			p_fd;
		bool 			nickname_set;  // To check if nickname & user are already set
 		bool 			user_set;
		bool			registered;
		void 			receive_from(Server *server);
		void 			dispatch();
		void 			write(std::string message);
		void 			push();
};

#endif	