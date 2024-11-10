#pragma once

#include "Command.hpp"

class Topic : public Command
{
	public:
		Topic();
		~Topic();
		virtual void execute(Client* client, std::list<string> args);
};