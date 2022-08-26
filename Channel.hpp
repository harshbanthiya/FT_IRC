/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 10:23:18 by olabrecq          #+#    #+#             */
/*   Updated: 2022/08/26 11:15:04 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class Channel
{
	private:
		std::string _ch_name;
		std::vector<Client *> 		clients_in_channel;
	public:
		Channel(std::string channel_name);
		~Channel( void );

		std::string get_ch_name( void );
		void set_ch_name( std::string new_ch_name );
};