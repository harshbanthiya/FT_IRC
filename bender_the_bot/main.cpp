/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 12:17:02 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/26 15:05:10 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#include "bender.hpp"

void	parse_args(int ac, char **av, std::string& server_ip, std::string& port, std::string& password)
{
	int i = 0;
	if (ac == 4)
		server_ip = av[i++];
	port = av[i++];
	password = av[i];
}


int main(int ac, char **argv)
{
    if (ac != 3 && ac != 4)
    {
        std::cerr << "Usage: " << argv[0] << " [server_ip] port password" << std::endl;
        return (1);
    }

    std::string server_ip = "127.0.0.1";
    std::string port;
    std::string password;
    parse_args(ac, &argv[1], server_ip, port, password);

    Bot *bot;
    try{
        bot = new Bot(server_ip, port, password);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error occured in creating bot: " << e.what() << std::endl;
        return (1);
    }
    	try
	{
		bot->run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error occured while the bot was running: " << e.what() << std::endl;
		return (1);
	}
	delete bot;
    return (1);
}
