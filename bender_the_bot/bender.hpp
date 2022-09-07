/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bender.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 12:17:08 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/07 15:51:20 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BENDER_HPP
#define BENDER_HPP 

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cerrno>
#include <vector>
#include <fstream>
#include <iostream>

#define INSULTS_FILEPATH "./bender_the_bot/insults.txt"
#define BOT_NAME std::string("benderBOT")
#define END_DELIM std::string("\r\n")

class Bot
{
    public:

    Bot(std::string host_ip, std::string port, std::string password);
    ~Bot();
    void run();

    private:
    
    std::string     server_ip;
    std::string     port;
    std::string     password;
    int             socket_fd;
    char            buff[512];
    std::vector<std::string> insults;

    int             _register();
    void            load_insults(const char *file);
    
    int			    get_numeric(std::string buff) const;
    std::string     get_cmd(std::string buff)const;
    std::string	    get_sender(std::string buff) const;
    std::string     get_text(std::string buff) const;
    
    void            send_msg(std::string buff)const;

    void            handle_cmd(std::string msg) const;
    void            handle_join()const;
    void            handle_privmsg()const;
		
};

#endif 