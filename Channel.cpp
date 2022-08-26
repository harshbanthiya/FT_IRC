/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 11:12:49 by olabrecq          #+#    #+#             */
/*   Updated: 2022/08/26 11:19:47 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"


Channel::Channel( std::string channel_name ) : _ch_name(channel_name)
{
	
}

Channel::~Channel() {

}

std::string Channel::get_ch_name( void ) { return _ch_name; }
void Channel::set_ch_name( std::string new_ch_name) { _ch_name = new_ch_name; }