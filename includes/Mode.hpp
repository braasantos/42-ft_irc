#pragma once

#include "Command.hpp"

class Mode : public Command
{
	public:
		Mode();
		~Mode();
		virtual void execute(Client* client, std::list<string> args);
};