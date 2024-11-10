#pragma once

#include "utils.hpp"
#include "Client.hpp"

class Command
{
	public:
		Command();
		virtual ~Command();
		virtual bool getPasswordRequired() const;
		virtual void execute(Client* client, std::list<string> args) = 0;
};