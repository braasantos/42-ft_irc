#include "../../includes/Part.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Part::Part() {}

Part::~Part() {}

void Part::execute(Client *client, std::list<string> args)
{
	if (!client->isAuthenticated())
	{
		ERR_AUTH(client);
		return;
	}

	if (args.size() < 1)
	{
		client->response(":" + client->getHostname() + " 461 PART :Not enough parameters\r\n");
		return;
	}

	Server *server = client->getServer();
	string reason;
	string channel;

	channel = args.front();
	args.pop_front();
	if (!args.empty())
	{
		while (!args.empty())
		{
			reason += args.front();
			args.pop_front();
			if (!args.empty())
				reason += " ";
		}
	}

	Channel *the_channel = server->getChannel(channel);

	if (the_channel == NULL)
	{
		client->response(":" + client->getHostname() + " 403 " + channel + " :No such channel\r\n");
		return ;
	}

	if (!the_channel->isOnChannel(client->getNickname()))
	{
		client->response(":" + client->getHostname() + " 442 " + channel + " :You're not on that channel\r\n");
		return ;
	}

	std::vector<Client *> members = the_channel->getMembers();
	for (std::vector<Client *>::iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt)
		(*memberIt)->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + channel + " :" + reason + "\r\n");

	the_channel->removeMember(client);
}