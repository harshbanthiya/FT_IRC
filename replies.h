/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sheeed <sheeed@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:21:32 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/25 18:57:26 by sheeed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
#define REPLIES_H

#define RPL_WELCOME             1
#define RPL_YOURHOST            2
#define RPL_CREATED             3
#define RPL_MYINFO              4


#define PRINT_SCREEN            372


#define ERR_NOSUCHNICK          401
#define ERR_NORECIPIENT         411
#define ERR_NOTEXTTOSEND        412
#define ERR_NONICKNAMEGIVEN     431
#define ERR_NICKNAMEINUSE       433
#define ERR_NEEDMOREPARAMS      461
#define ERR_ALREADYREGISTERED   462
#define ERR_PASSWDMISMATCH      464

#define END_DELIM       "\r\n"
/*
std::string ERR_NOORIGIN() { return ":No origin specified"; }
std::string ERR_NONICKNAMEGIVEN() { return ":No nickname given"; }
std::string ERR_ERRONEUSNICKNAME(std::string nick) { return nick + " :Erroneus nickname"; }
std::string ERR_NICKNAMEINUSE(std::string nick) { return nick + " :Nickname is already in use"; }
std::string RPL_PRINTWELCOME(std::string text) { return "- " + text; }
*/
#endif 