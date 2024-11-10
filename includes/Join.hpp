#pragma once

#include "Command.hpp"

class Join : public Command
{
	public:
		Join();
		~Join();
		virtual void execute(Client* client, std::list<string> args);
};