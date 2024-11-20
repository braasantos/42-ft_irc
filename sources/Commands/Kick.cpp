#include "../../includes/Kick.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"

Kick::Kick()
{}

Kick::~Kick()
{}

void Kick::execute(Client* client, std::list<string> args)
{
	if (!client->isAuthenticated())
	{
		ERR_AUTH(client);
		return;
	}

	if (args.size() < 2)
	{
		ERR_NORECIPIENT(client, "KICK");
		return ;
	}
	string currChannel = args.front();
	string targetToKick;
	string reason;
	args.pop_front();
	targetToKick += args.front();
	args.pop_front();
	if (targetToKick.empty())
	{
		ERR_NOTEXTTOSEND(client);
		return;
	}
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
	else
		reason = "No specific reson";
	Server *server = client->getServer();
	Channel *channel = server->getChannel(currChannel);
	if (!channel)
	{
		ERR_NOSUCHCHANNEL(client, currChannel);
		return;
	}
	if (!channel->isOperator(client))
	{
		ERR_NOTANOPERATOR(client, currChannel);
		return ;
	}
	if (currChannel[0] == '#' || currChannel[0] == '&')
	{
		Channel *channel = server->getChannel(currChannel);
		if (channel == NULL)
		{
			ERR_NOSUCHCHANNEL(client, currChannel);
			return;
		}
		if (!channel->isOnChannel(client->getNickname()))
		{
			ERR_NOSUCHNICKONCH(client, channel->getName());
			return;
		}
		if (!channel->isOnChannel(targetToKick))
		{
			ERR_NOSUCHNICKONCH(client, targetToKick);
			return;
		}
	}
	if (targetToKick == client->getNickname())
	{
		ERR_CANNOTKICKYOURSELF(client);
		return ;
	}

	std::list<string> channels;
	channels.push_back(channel->getName());

	for (std::list<string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		std::vector<Client *> members = channel->getMembers();
		for (std::vector<Client *>::iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt)
		{
			(*memberIt)->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " KICK " + currChannel + " " + targetToKick + " :" + reason + "\r\n");
			if ((*memberIt)->getNickname() == targetToKick)
				channel->removeMember(*memberIt);
		}
	}
}