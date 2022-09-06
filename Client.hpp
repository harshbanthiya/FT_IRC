/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 18:53:47 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/06 13:39:50 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <vector>

class Client
{
		public:

			// Constructors 
			Client(int fd, std::string host);
			~Client();
			
			std::string& 					buffer();
			bool 							is_passed()const;
			bool 							is_registered()const;
			
			
			//Channel stuff
			void 							add_channel(std::string name);
			void 							remove_channel(std::string name);
			
			// Modes
			bool							has_mode(char mode) const;
			void 							add_mode(char mode);
			void 							del_mode(char mode);


			// Getters 
			
			std::string const	 			&get_hostname(void) const;
			std::string	const 				&get_nickname(void) const;
			std::string	const 				&get_username(void) const;
			std::string	const 				&get_realname(void) const;
			bool 							get_registered(void);
			int 							get_socket()const;
			std::string const 				&get_modes()const;
			std::vector<std::string> const 	&get_channels() const;

			// Setters 
	
			void 							set_nickname(std::string nick_n);
			void 							set_username(std::string user_n);
			void 							set_hostname(std::string host_n);
			void 							set_realname(std::string real_n);
			void 							set_registered();
			void 							set_passed();
		
			bool							operator==(Client const & other) const;
			bool							operator==(std::string const & other) const;
			
		private:
		
			int 							socket_fd;
			std::string 					hostname;
			bool 							pass_set;
			bool							registered;
			std::string 					nickname;
			std::string 					username;
			std::string 					realname;
			std::string 					_buffer;
			std::string 					modes;
			std::vector<std::string>		channels;
	};
	std::string		toUpper(std::string const & str);

#endif	