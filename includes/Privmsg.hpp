#pragma once

#include "Command.hpp"

class Privmsg : public Command
{
	public:
		Privmsg();
		~Privmsg();
		virtual void execute(Client* client, std::list<string> args);
};