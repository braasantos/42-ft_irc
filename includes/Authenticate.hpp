#pragma once

#include "Command.hpp"

class Authenticate : public Command
{
	public:
		Authenticate();
		~Authenticate();
		virtual void execute(Client* client, std::list<string> args);
};