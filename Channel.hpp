/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:47:31 by sheeed            #+#    #+#             */
/*   Updated: 2022/09/15 15:55:55 by hbanthiy         ###   ########.fr       */
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

class Server;

class Channel
{
	public:
	
		Channel();
		Channel(std::string name, Server &_serv);
		Channel(std::string name, std::string key, Server &serv);
		Channel(Channel const & channel);
		~Channel();

		void 					add_client( Client &new_client, std::string key, char status);
		bool					remove_client(Client &client);
		bool 					remove_client(std::string const & nick);
		void 					make_user_part(Client &cl);
		void					send_to_all(std::string msg, std::string sender = "") const;
		bool 					is_user_in_channel(Client const &user)const;
		bool 					is_user_in_channel(std::string nickname)const;

		bool					add_mode(Client &owner, char m, char mode, std::string param = "");
		void 					add_mode(char mode);
		void					delete_mode(char mode);
		bool 					is_operator(Client const &owner) const;
		bool 					is_operator(std::string const &owner) const;
		bool					empty();
		void					kick(Client &client, std::list<std::string> &clients, std::string msg = "Byeeeeeee bitch!");
		
		void					invite(Client &owner, std::string nick);
		bool					is_banned(Client const &owner) const;
		bool					is_banned(std::string const &owner) const;
		bool					can_join(Client const &owner) const;
		bool 					can_send_msg(Client const &owner) const;
		bool					is_invited(Client const &owner) const;
		bool					is_invited(std::string const &owner) const;

		void					ban(Client &owner, std::string nick);
		void					unBan(Client &owner, std::string nick);
		void					send_ban_list(Client &owner) const;

		bool 					mode_invite(Client &owner, char mode);
		bool 					mode_ban(Client &owner, char mode, std::string params);
		bool 					mode_key(Client &owner, char mode, std::string params);
		bool 					mode_operator(Client &owner, char mode, std::string params);
		bool					mode_topic(Client &owner, char mode);

		
		std::string				get_str_clients() const;
		std::string const		&get_modes()const;
		std::string  			get_name(bool ck = false) const;
		std::string  			get_key(void) const;
		std::string  			get_topic() const;
		void 					get_topic(Client &owner) const;
		std::string  			get_topic_time() const;
		std::string 			get_creation_time() const;
		size_t					get_user_count()const;

		const std::vector<std::pair<char,Client *> > &get_client_list()const;
		const std::set<std::string> &get_ban_list()const;

		
		void	set_topic( Client &owner, std::string &topic );


	private:
		std::string                             _name;
		std::string                             _key;
		std::string                             _topic;
		std::string 							_topic_setter;
		std::time_t                             _topic_time;
		std::time_t                             _creation_time;
		Server                                  *_serv;
		std::string                             _modes;
		int 									_limit;

		std::vector<std::pair<char, Client*> >  _clients;
		std::set<std::string>  					_invite_list;
		std::set<std::string>					_ban_list;

};

Client 	*join_channel(std::string arg);
#endif
