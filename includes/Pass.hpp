#pragma once

#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Pass : public Command
{
	public:
		Pass();
		~Pass();
		virtual void execute(Client* client, std::list<string> args);
};