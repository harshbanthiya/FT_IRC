/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bender.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 12:17:12 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/08 16:03:06 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bender.hpp"

Bot::Bot(std::string _server_ip, std::string _port, std::string _password) :
server_ip(_server_ip), port(_port), password(_password)
{
    struct addrinfo hints, *ai;
    int ret, yes = 1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    if ((ret = getaddrinfo(server_ip.c_str(), port.c_str(), &hints, &ai)) != 0)
        throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(ret)));
    if ((this->socket_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0)
        throw std::runtime_error("socket: " + std::string(strerror(errno)));
    setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (connect(this->socket_fd , ai->ai_addr, ai->ai_addrlen) == -1)
    {
        close(this->socket_fd);
        throw std::runtime_error("connect: " + std::string(strerror(errno)));
    }
    freeaddrinfo(ai);
    load_insults(INSULTS_FILEPATH);
}

Bot::~Bot() {}

void Bot::load_insults(const char *file)
{
    std::fstream insult_file;
    insult_file.open(file, std::ios::in);
    if (!insult_file)
        throw std::runtime_error("Open : no file");
    insult_file >> std::noskipws;
    std::string     line;
    char            ch;

    do{
        insult_file >> ch;
        if (ch == '\n')
        {
            insults.push_back(line);
            line.clear();
        }
        else 
            line += ch;
    }while (!insult_file.eof());
	insult_file.close();
}

void Bot::run()
{
    int ret = _register();
    if (ret != 1)
    {
        close(this->socket_fd);
        if (!ret)
            throw std::runtime_error("Connection to server lost");
        if (ret < 0)
            throw std::runtime_error("recv(): " + std::string(strerror(errno)));
        if (ret == 464)
            throw std::runtime_error("Incorrect password");
    }
    else 
        std::cout << "Bender correctly connected!" << std::endl;
    while (1)
    {
        memset(this->buff, 0, sizeof(this->buff));
        int nbytes = recv(this->socket_fd, this->buff, sizeof(this->buff), 0);
        if (!nbytes)
            throw std::runtime_error("Connection to server lost!");
        if (nbytes < 0)
            throw std::runtime_error("recv() :" + std::string(strerror(errno)));
        handle_cmd(get_cmd(this->buff));
    }
}

int Bot::get_numeric(std::string buff) const
{
    buff.erase(0, buff.find(" ") + 1);
    std::string numeric = buff.substr(0,buff.find(" "));
    return (std::atoi(numeric.c_str()));
}

int Bot::_register()
{
    std::string     msg;
    int             nbytes;

    sleep(1);
    msg = "PASS " + this->password + END_DELIM;
    send_msg(msg);
    msg = "NICK " + BOT_NAME + END_DELIM;
    send_msg(msg);
    msg = "USER bot_user 0 * :bender BOT" + END_DELIM;
    send_msg(msg);
    msg = "JOIN #" + BOT_NAME + END_DELIM;
    send_msg(msg);
    memset(this->buff, 0, sizeof(buff));
    nbytes = recv(this->socket_fd, this->buff, sizeof(this->buff), 0);
    //std::cout << buff << '\n';
    if (nbytes <= 0)
        return (nbytes);
    int num = get_numeric(this->buff);
    return (num);
}

std::string Bot::get_cmd(std::string buff) const
{
    buff.erase(0, buff.find(" ") + 1); 
    return (buff.substr(0, buff.find(" ")));
}

void    Bot::handle_cmd(std::string cmd) const
{
    if (cmd == "PRIVMSG")
        handle_privmsg();
    if (cmd == "JOIN")
        handle_join();
    if (cmd == "PART")
        handle_part();
}

void    Bot::send_msg(std::string msg) const
{
    send(this->socket_fd, msg.c_str(), msg.length(), 0);
}

void    Bot::handle_join() const
{
    std::string sender = get_sender(this->buff);
    if (sender == BOT_NAME)
        return ;
    std::string header = "PRIVMSG #" + BOT_NAME + " :";
    std::string msg = header + sender + " GET READY TO BE ROASTED HUMAN!" + END_DELIM;
    send_msg(msg);
    msg = header + "Commands available :: ROASTME / ROAST <NAME> /COMMAND " + END_DELIM;
    send_msg(msg);
}

void    Bot::handle_privmsg() const
{
    std::string   text = get_text(this->buff);
    srand(time(nullptr));
    std::string msg = "PRIVMSG #" + BOT_NAME + " :";
    if (text == ":COMMAND" + END_DELIM || text.substr(0, text.find(" ")) == ":COMMAND")
        msg += "Commands available :: ROASTME /  ROAST <NAME> / COMMAND" + END_DELIM;
    else
    {
        std::string insult = insults[rand() % this->insults.size()];
        std::string sender = get_sender(this->buff);
        if (text == ":ROASTME" + END_DELIM || text.substr(0, text.find(" ")) == ":ROASTME")
            msg += sender + ", " + insult + END_DELIM;
        else if (text == ":ROAST" + END_DELIM || text.substr(0, text.find(" ")) == ":ROAST")
        {
            int pos = std::string(text).find(" ");
            if (pos != -1)
            {
                sender = std::string(text).substr(pos);
                sender.pop_back();
                sender.pop_back();
            }
            msg += sender + ", " + insult + END_DELIM;
        }

    }   
    if (msg != ("PRIVMSG #" + BOT_NAME + " :"))
        send_msg(msg);
    
}

void    Bot::handle_part()const
{
    std::string msg = "PRIVMSG #" + BOT_NAME + " :";
    std::string sender = get_sender(this->buff);
    msg += sender + "HASTA LA VISTA BABY" + END_DELIM;
    send_msg(msg);
}

std::string Bot::get_text(std::string buff) const
{
    int pos = buff.find(" ") + 1; // source 
    pos = buff.find(" ", pos) + 1; // Priv msg
    pos = buff.find(" ", pos) + 1;  // Bender 
    return (buff.substr(pos));
}

std::string	Bot::get_sender(std::string buff) const
{
	buff.erase(0, 1); // deleting first ":"
	return (buff.substr(0, buff.find("!")));
}