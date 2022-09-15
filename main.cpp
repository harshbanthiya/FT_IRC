/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 12:18:18 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/14 14:56:59 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>


int main(int argc, char **argv)
{
	Server *server = NULL;
    if (argc != 3) 
	{
        std::cout << "Usage: ./irc <port> <password> \n";
        return (1);
    }
    try 
	{
		server = new Server(argv[1], argv[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error occured in creating server: " << e.what() << std::endl;
		return (1);
	}
	try
	{
        server->execute();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error occured while the server was running \n" << e.what() << std::endl;
	}
	delete(server);
    return (0);
}