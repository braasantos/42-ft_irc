#include "../../includes/Who.hpp"
#include "../../includes/Channel.hpp"

Who::Who() {}

Who::~Who() {}

void Who::execute(Client *client, std::list<std::string> args)
{
    if (args.empty())
    {
        ERR_NEEDMOREPARAMS(client, "WHO");
        return;
    }

    Server *server = client->getServer();
    std::string mask = args.front();
    args.pop_front();

    if (mask[0] == '#')
    {
        Channel *channel = server->getChannel(mask);
        if (channel != NULL)
        {
            std::vector<Client*>::iterator it;
            std::vector<Client*> members = channel->getMembers();

            for (it = members.begin(); it != members.end(); ++it)
            {
                Client *member = *it;
                RPL_WHOREPLY(member, server, mask);
            }
        }
    }
    else
    {
        std::map<int, Client *>clients = server->getClients();
        std::map<int, Client *>::iterator it;

        for (it = clients.begin(); it != clients.end(); ++it)
        {
            Client *targetClient = it->second;
            if (targetClient->getNickname() == mask)
                RPL_WHOREPLY(targetClient, server, "*");
        }
    }
    RPL_ENDOFWHO(client, mask);
}