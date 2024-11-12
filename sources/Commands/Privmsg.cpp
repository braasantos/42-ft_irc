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
	if (args.size() < 2)
	{
		client->response(":" + client->getHostname() + " 411 :No recipient given (PRIVMSG)\r\n");
		return;
	}

	string target = args.front();
	args.pop_front();
	string message;
	while (!args.empty())
	{
		message += args.front() + " ";
		args.pop_front();
	}
	if (message.empty())
	{
		client->response(":" + client->getHostname() + " 412 :No text to send\r\n");
		return;
	}

	Server *server = client->getServer();

	if (target[0] == '#' || target[0] == '&')
	{
		Channel *channel = server->getChannel(target);
		if (channel == NULL)
		{
			client->response(":" + client->getHostname() + " 403 " + target + " :No such channel\r\n");
			return;
		}

		// Check if the client is banned from the channel
		if (!channel->isOnChannel(client->getNickname()))
		{
			client->response(":" + client->getHostname() + " 404 " + target + " :Cannot send to channel\r\n");
			return;
		}

		std::vector<Client *> members = channel->getMembers();
		for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
			(*it)->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PRIVMSG " + target + " :" + message + "\r\n");
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
			client->response(":" + client->getHostname() + " 401 " + target + " :No such nick/channel\r\n");
			return;
		}
		targetClient->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PRIVMSG " + target + " :" + message + "\r\n");
	}
}