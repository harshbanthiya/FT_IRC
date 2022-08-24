/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:21:32 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/24 13:22:00 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
#define REPLIES_H

#define RPL_WELCOME     1
#define RPL_YOURHOST    2
#define RPL_CREATED     3
#define RPL_MYINFO      4
#define END_DELIM       "\r\n"
/*
std::string ERR_NOORIGIN() { return ":No origin specified"; }
std::string ERR_NONICKNAMEGIVEN() { return ":No nickname given"; }
std::string ERR_ERRONEUSNICKNAME(std::string nick) { return nick + " :Erroneus nickname"; }
std::string ERR_NICKNAMEINUSE(std::string nick) { return nick + " :Nickname is already in use"; }
std::string RPL_PRINTWELCOME(std::string text) { return "- " + text; }
*/
#endif 