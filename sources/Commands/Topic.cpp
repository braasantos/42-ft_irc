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
		cout << "NO ARGUMENTS\r\n";
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
			cout << channel->getTopic() << endl;
			return ;
		}
		cout << "NO TOPIC SET\r\n";
		return ;
	}
	string topic = args.front();
	args.pop_front();
	if (topic[0] != ':')
	{
		cout << "ERROR\r\n";
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
			cout << "NOT OPERATOR\r\n";
			return ;
		}
	}
	else
	{
		channel->setTopic(topic.substr(1));
	}
}