#pragma once

#include "Command.hpp"

class Part : public Command
{
	public:
		Part();
		~Part();
		virtual void execute(Client* client, std::list<string> args);
};