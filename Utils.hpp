/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbanthiy <hbanthiy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 13:35:32 by hbanthiy          #+#    #+#             */
/*   Updated: 2022/08/19 13:55:09 by hbanthiy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

#ifndef DEBUG
#define DEBUG 0
#endif


namespace irc
{
    std::string     currentTime();

    
    std::vector<std::string> split(std::string str, std::string delimiter);

    bool            isLetter(char c);
    bool            isSpecial(char c);
    bool            isDigit(char c);
    
}

#endif 