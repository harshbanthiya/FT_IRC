/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:47:31 by sheeed            #+#    #+#             */
/*   Updated: 2022/09/01 14:50:14 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <stdexcept>
#include <vector>
#include <list>
#include <set>
#include <iostream>


#include "Client.hpp"
#include "Command_Handler.hpp"

class Server;

class Channel
{
	public:

		// Will need a user_list , a ban_list and an invite list 
		Channel();
		Channel(std::string name, Server &_serv);
		Channel(std::string name, std::string key, Server &serv);
		~Channel();

		std::string  	get_name(bool ck = false) const;
		std::string  	get_key(void) const { return _key; }
		std::string  	get_topic(void) const { return _topic; }
		// std::string		get_modes(void) const { return _modes; }
		Server* 		get_server(void) const { return _serv; }
		bool 			is_user_in_channel(std::string nickname)const;
		const std::vector<std::pair<char, Client *> > &getClients() const;
		std::string		get_str_clients() const;
		//void 	message_all_users();

		void	send_to_all(std::string msg) const; // Needs to be added
		bool 	canSendMsg(Client const &owner) const; // Also needs to be added 
		void 	add_client( Client &new_client, std::string key, char status);

		// Mode stuff
		std::string const	&get_modes()const;
		std::string 		get_creation_time() const;
		bool				add_mode(Client &owner, char m, char mode, std::string param = "");
		void 				add_mode(char mode);
		bool 				is_operator(Client const &owner) const;

		// Actual Mode functions 
		//bool 				mode_invite(Client &owner, char mode);
		bool 				mode_ban(Client &owner, char mode, std::string params);
		bool 				mode_operator(Client &owner, char mode, std::string params);

		void	set_name( std::string name ) { _name = name; }
		void	set_key( std::string key ) { _key = key; }
		void	set_topic( std::string topic ) { _topic = topic; }
		void	set_modes( std::string modes ) { _modes = modes; }

	private:
		std::string                             _name;
		std::string                             _key;
		std::string                             _topic;
		std::time_t                             _topic_time;
		std::time_t                             _creation_time;
		Server                                  *_serv;
		std::string                             _modes;
		std::vector<std::pair<char, Client*> >  _clients;
};





#endif
