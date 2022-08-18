/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADMIN.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olabrecq <olabrecq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 10:52:51 by olabrecq          #+#    #+#             */
/*   Updated: 2022/08/18 13:35:39 by olabrecq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include "Client.hpp"

void ADMIN(irc::Command *command) {
    if (!command->getParameters().size()) {
        command->reply(256); //     RPL_ADMINME
        command->reply(257); //     RPL_ADMINLOC1
        command->reply(258); //     RPL_ADMINLOC2
        command->reply(259);//      RPL_ADMINEMAIL
        return;
    }
}