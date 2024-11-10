#pragma once

#include "Command.hpp"

class Kick : public Command
{
	public:
		Kick();
		~Kick();
		virtual void execute(Client* client, std::list<string> args);
};