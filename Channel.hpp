/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:47:31 by sheeed            #+#    #+#             */
/*   Updated: 2022/08/26 13:28:39 by olabrecq         ###   ########.fr       */
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
    Channel(std::string name, Server &serv);
    Channel(std::string name, std::string key, Server &serv);
    ~Channel();
	bool is_user_in_channel(std::string nickname);



    private: 
    std::string                             name;
    std::string                             key;
    std::string                             topic;
    std::time_t                             topic_time;
    std::time_t                             creation_time;
    Server                                  *serv;
    std::string                             modes;
    std::vector<std::pair<char, Client*> >  clients;
public:
	const std::vector<std::pair<char, Client *> > &getClients() const;
};





#endif
