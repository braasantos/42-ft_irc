#include "../../includes/Topic.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Topic::Topic()
{
}

Topic::~Topic()
{
}

void Topic::execute(Client *client, std::list<string> args)
{
    if (!client->isAuthenticated())
    {
        ERR_AUTH(client);
        return;
    }
    if (args.empty())
    {
        ERR_NEEDMOREPARAMS(client, "TOPIC");
        return ;
    }

    if (args.size() == 1)
    {
        string ch_name = args.front();
        if (ch_name[0] == '#')
        {
            Channel *channel = client->getServer()->getChannel(ch_name);
            if (channel == NULL)
            {
                ERR_NOSUCHCHANNEL(client, ch_name);
                return ;
            }
            if (!channel->getTopic().empty())
            {
                client->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " TOPIC " + ch_name + " :" + channel->getTopic() + "\r\n");
				return ;
            }
            RPL_NOTOPIC(client, ch_name);
            return ;
        }
    }

    string ch_name = args.front();
    args.pop_front();

    Channel *channel = client->getServer()->getChannel(ch_name);
    
    string topic;

    while (!args.empty())
    {
        topic += args.front();
        args.pop_front();
        if (!args.empty())
            topic += " ";
    }

    if (topic[0] != ':')
    {
        ERR_INVALIDTOPIC(client, ch_name);
        return ;
    }

    if (std::strcmp(topic.c_str(), "::") == 0)
    {
        channel->setTopic("");
        return ;
    }
    if (channel->getMode() == 't')
    {
        if (channel->isOperator(client))
        {
            channel->setTopic(topic.substr(1));
            return ;
        }
        else
        {
            ERR_NOTANOPERATOR(client, ch_name);
            return ;
        }
    }
    else
        channel->setTopic(topic.substr(1));
}