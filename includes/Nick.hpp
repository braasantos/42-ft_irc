#pragma once

#include "Command.hpp"

class Nick : public Command
{
	public:
		Nick();
		~Nick();
		virtual void execute(Client* client, std::list<string> args);
};