#pragma once

#include "Command.hpp"

class Quit : public Command
{
	public:
		Quit();
		~Quit();
		virtual void execute(Client* client, std::list<string> args);
};