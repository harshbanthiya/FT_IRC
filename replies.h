/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:21:32 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/31 12:46:10 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
#define REPLIES_H

#define RPL_WELCOME             1
#define RPL_YOURHOST            2
#define RPL_CREATED             3
#define RPL_MYINFO              4

#define RPL_ADMINME 			256
#define RPL_ADMINLOC1 			257
#define RPL_ADMINLOC2 			258
#define RPL_ADMINEMAIL 			259

#define RPL_AWAY				301
#define RPL_ENDOFWHO			315
#define RPL_CHANNELMODEIS       324
#define RPL_CREATIONTIME		329  
#define RPL_WHOREPLY			352
#define RPL_NAMREPLY 			353
#define RPL_ENDOFNAMES			366
#define RPL_ENDOFBANLIST		368
#define PRINT_SCREEN            372
#define RPL_TIME 				391


#define ERR_NOSUCHNICK          401
#define ERR_NOSUCHCHANNEL		403
#define ERR_NORECIPIENT         411
#define ERR_NOTEXTTOSEND        412
#define ERR_NONICKNAMEGIVEN     431
#define ERR_NICKNAMEINUSE       433
#define ERR_NEEDMOREPARAMS      461
#define ERR_ALREADYREGISTERED   462
#define ERR_PASSWDMISMATCH      464
#define ERR_UNKNOWNMODE			472
#define ERR_CHANOPRIVSNEEDED	482

#define END_DELIM       "\r\n"
/*
std::string ERR_NOORIGIN() { return ":No origin specified"; }
std::string ERR_NONICKNAMEGIVEN() { return ":No nickname given"; }
std::string ERR_ERRONEUSNICKNAME(std::string nick) { return nick + " :Erroneus nickname"; }
std::string ERR_NICKNAMEINUSE(std::string nick) { return nick + " :Nickname is already in use"; }
std::string RPL_PRINTWELCOME(std::string text) { return "- " + text; }
*/
#endif 

