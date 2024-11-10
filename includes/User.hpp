#pragma once

#include "Command.hpp"

class User : public Command
{
	public:
		User();
		~User();
		virtual void execute(Client* client, std::list<string> args);
};