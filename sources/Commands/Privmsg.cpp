#include "../../includes/Privmsg.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

Privmsg::Privmsg()
{
}

Privmsg::~Privmsg()
{
}

void Privmsg::execute(Client *client, std::list<string> args)
{

	if (!client->isAuthenticated())
	{
		ERR_AUTH(client);
		return;
	}

	if (args.empty())
	{
		ERR_NORECIPIENT(client, "PRIVMSG");
		return;
	}

	string target = args.front();
	string message;

	args.pop_front();

	while (!args.empty())
	{
		message += args.front() + " ";
		args.pop_front();
	}

	if (message.empty())
	{
		ERR_NOTEXTTOSEND(client);
		return;
	}

	Server *server = client->getServer();
	if (target[0] == '#' || target[0] == '&')
	{
		Channel *channel = server->getChannel(target);
		if (channel == NULL)
		{
			ERR_NOSUCHNICK(client, target);
			return;
		}
		if (!channel->isOnChannel(client->getNickname()))
		{
			ERR_CANNOTSENDTOCHAN(client, channel->getName());
			return;
		}
		std::vector<Client *> members = channel->getMembers();
		std::vector<Client *>::iterator it = members.begin();
		for (; it != members.end(); ++it)
		{
			if (*it != client)
				(*it)->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PRIVMSG " + target + " :" + message + "\r\n");
		}
	}
	else
	{
		const std::map<int, Client *> &clients = server->getClients();
		Client *targetClient = NULL;
		for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (it->second->getNickname() == target)
			{
				targetClient = it->second;
				break;
			}
		}

		if (targetClient == NULL)
		{
			ERR_NOSUCHNICK(client, target);
			return;
		}
		targetClient->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PRIVMSG " + target + " :" + message + "\r\n");
	}
}