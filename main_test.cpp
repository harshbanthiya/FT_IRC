/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 12:18:18 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/23 10:21:36 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <csignal>

bool stop = false;
void handler(int) { stop = true;}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./irc <port> <password> \n";
        return (1);
    }
    try 
	{
	    irc::Server server(atoi(argv[1]), argv[2]);
        signal(SIGINT, handler);
        server.init();  
		while (!stop)
        	server.execute();
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}
    return (0);
}