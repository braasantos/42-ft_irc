#pragma once

#include "Command.hpp"
#include "CommandHandler.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "utils.hpp"

class Who : public Command
{
    public:
        Who();
        ~Who();
        virtual void execute(Client *client, std::list<string> args);
};