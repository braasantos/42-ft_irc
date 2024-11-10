#pragma once

#include "Command.hpp"

class Invite : public Command
{
	public:
		Invite();
		~Invite();
		virtual void execute(Client* client, std::list<string> args);
};