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
	string chName = args.front();
	args.pop_front();
	Server *server = client->getServer();
	Channel *channel = server->getChannel(chName);
	if (args.empty())
	{
		if (!channel->getTopic().empty())
		{
			RPL_TOPIC(client, channel);
			return ;
		}
		RPL_NOTOPIC(client, chName);
		return ;
	}
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
		ERR_INVALIDTOPIC(client, chName);
		return ;
	}

	if (std::strcmp(topic.c_str(), ":") == 0)
	{
		channel->setTopic("");
		return ;
	}

	if (channel->getMode() == 't')
	{
		if (channel->isOperator(client))
		{
			channel->setTopic(topic.substr(1));
			cout << channel->getTopic() << endl;
			return ;
		}
		else
		{
			ERR_NOTANOPERATOR(client, chName);
			return ;
		}
	}
	else
		channel->setTopic(topic.substr(1));
}