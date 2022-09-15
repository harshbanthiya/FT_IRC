/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:21:32 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/09/14 15:54:14 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_H
#define REPLIES_H

#define RPL_WELCOME             1
#define RPL_YOURHOST            2
#define RPL_CREATED             3
#define RPL_MYINFO              4

#define RPL_UMODEIS			    221 
#define RPL_ADMINME 			256
#define RPL_ADMINLOC1 			257
#define RPL_ADMINLOC2 			258
#define RPL_ADMINEMAIL 			259

#define RPL_AWAY				301
#define RPL_ENDOFWHO			315
#define RPL_CHANNELMODEIS       324
#define RPL_CREATIONTIME		329  
#define RPL_NOTOPIC			    331 
#define RPL_TOPIC               332
#define RPL_TOPICWHOTIME		333 
#define RPL_INVITING			341 
#define RPL_WHOREPLY			352
#define RPL_NAMREPLY 			353
#define RPL_ENDOFNAMES			366
#define RPL_BANLIST             367
#define RPL_ENDOFBANLIST		368
#define RPL_MOTD				372 
#define RPL_MOTDSTART			375 
#define RPL_ENDOFMOTD			376 
#define RPL_TIME 				391

#define ERR_NOSUCHNICK          401
#define ERR_NOSUCHSERVER		402 
#define ERR_NOSUCHCHANNEL		403
#define ERR_CANNOTSENDTOCHAN	404 
#define ERR_NORECIPIENT         411
#define ERR_NOTEXTTOSEND        412
#define ERR_UNKNOWNCOMMAND		421 
#define ERR_NOMOTD				422 
#define ERR_NONICKNAMEGIVEN     431
#define ERR_NICKNAMEINUSE       433
#define ERR_USERNOTINCHANNEL	441  
#define ERR_NOTONCHANNEL		442  
#define ERR_USERONCHANNEL		443  
#define ERR_NOTREGISTERED		451 
#define ERR_NEEDMOREPARAMS      461
#define ERR_ALREADYREGISTERED   462
#define ERR_PASSWDMISMATCH      464
#define ERR_UNKNOWNMODE		    472
#define ERR_INVITEONLYCHAN		473 
#define ERR_BANNEDFROMCHAN      474
#define ERR_BADCHANNELKEY		475 
#define ERR_CHANOPRIVSNEEDED	482

#define ERR_UMODEUNKNOWNFLAG	501 
#define ERR_USERSDONTMATCH		502
#define ERR_INVALIDMODEPARAM	696 

#define END_DELIM               "\r\n"

#endif 

