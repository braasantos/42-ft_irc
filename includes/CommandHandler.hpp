#pragma once

#include "utils.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
#include "Nick.hpp"
#include "Part.hpp"
#include "Pass.hpp"
#include "Privmsg.hpp"
#include "Quit.hpp"
#include "Topic.hpp"
#include "User.hpp"
#include "Authenticate.hpp"
#include "Server.hpp"

class CommandHandler
{
	private:
		std::map<string, Command*> _commands;

	public:
		CommandHandler();
		~CommandHandler();

		void handleCommand(string command, Client *client);
};
