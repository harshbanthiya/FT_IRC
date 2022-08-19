/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 13:34:27 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/19 14:45:22 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Utils.hpp"
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <sstream>


std::string irc::currentTime()
{
	time_t t = std::time(0);
	struct tm *now = std::localtime(&t);
	std::string time(asctime(now));
	time.erase(--time.end());
	return time;
}

bool irc::isLetter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
bool irc::isSpecial(char c) { return (c >= '[' && c <= '`') || (c >= '{' && c <= '}'); }
bool irc::isDigit(char c) { return (c >= '0' && c <= '9'); }

std::vector<std::string> irc::split(std::string str, std::string delimiter)
{
	std::vector<std::string> values = std::vector<std::string>();

	size_t position;
	while ((position = str.find(delimiter)) != std::string::npos)
	{
		values.push_back(str.substr(0, position));
		str.erase(0, position + delimiter.length());
	}
	values.push_back(str);

	return values;
}

