#pragma once


#include "Command.hpp"
#include "CommandHandler.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "utils.hpp"

class Ping : public Command
{
    public:
        Ping();
        ~Ping();
        virtual void execute(Client *client, std::list<string> args);
};