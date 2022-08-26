/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:56:40 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/26 11:28:41 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"

Channel::Channel()
{
    creation_time = std::time(nullptr);
}

Channel::Channel(std::string _name, Server &_serv) :
    name(_name), key(""), topic(""), serv(&_serv)
{
    creation_time = std::time(nullptr);
    topic_time    = std::time(nullptr);
    
}

Channel::~Channel()
{
}