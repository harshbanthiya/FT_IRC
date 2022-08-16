//
// Created by Louis-gabriel Laplante on 8/16/22.
//

#include <iostream>
#include <string>
#define RPL_ADMIMME 256
#define RPL_ADMINLOC1 257
#define RPL_ADMINLOC2 258
#define RPL_ADMINEMAIL 259

#define adminme "Administrative info for ft_irc"
#define name "Server - FT_IRC"
#define nickname "Administrator: The Architect"
#define email "admin@irc.42.fr"

const char *build_reply(char *buf, int rpl_code, typename parameter)
{
	//buf should already be 4096 chars usable

}

void ADMIN(typename server, typename parameters, int socket) //so we know which socket to communicate on. All commands need these specific parameters to be compatible with function pointers
{
	//This command will take NO parameters. if there is one, ignore it (thus, we only return info about the current server
	//each line ends with CRLF "\r\n"
	char *buf = (char *) malloc(4096 * sizeof(char));

}