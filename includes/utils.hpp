/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmeirele <dmeirele@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 08:31:42 by dmeirele          #+#    #+#             */
/*   Updated: 2024/11/09 23:13:38 by dmeirele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iomanip>
#include <climits>
#include <exception>
#include <algorithm>
#include <vector>
#include <list>
#include <stack>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cctype>
#include <ranges>
#include <cstring>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <poll.h>
#include <netdb.h>
#include <csignal>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

#define RED "\033[31m"
#define GREEN "\033[32m"
#define ORANGE "\033[38;5;208m"
#define PURPLE "\e[35m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"


#define ERR_ALREADYREGISTERED(client) client->response(client->getHostname() + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(client) client->response(client->getHostname() + " " + ":Password incorrect\r\n");
#define ERR_NEEDMOREPARAMS(client, command) client->response(client->getHostname() + " " + command + " :Not enough parameters\r\n")
#define ERR_NONICKNAMEGIVEN(client) client->response(client->getHostname() + ":No nickname given");
#define ERR_ERRONEUSNICKNAME(client, nickname) client->response(client->getHostname() + nickname + ":Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(client, nickname) client->response(client->getHostname() + nickname + ":Nickname is already in use\r\n")
#define ERR_NOSUCHCHANNEL(client, ch_name) client->response(client->getHostname() + client->getNickname() + " " + ch_name + " :No such channel\r\n")
#define ERR_BADCHANNELKEY(client, ch_name) client->response(client->getHostname() + ch_name + ":Cannot join channel (+k)\r\n");
#define ERR_INVITEONLYCHAN(client, ch_name) client->response(client->getHostname() + ch_name + ":Cannot join channel (+i)\r\n");
#define ERR_TOOMANYCHANNELS(client, ch_name) client->response(client->getHostname() + ch_name + ":You have joined too many channels\r\n");

#define ERROR(client) client->response("ERROR :Connection timeout ;Server closing a client connection because it is unresponsive.\r\n");
#define ERR_AUTH(client) client->response("ERROR :Access denied: Authentication required\r\n");

#define ERR_PASSREQUIRED(client) client->response("ERROR :You need to setup password before use this command\r\n");