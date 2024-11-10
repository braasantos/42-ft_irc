#include "../../includes/Part.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Part::Part() {}

Part::~Part() {}

void Part::execute(Client *client, std::list<string> args)
{
	if (args.size() < 1)
	{
		client->response(":" + client->getHostname() + " 461 PART :Not enough parameters\r\n");
		return;
	}

	Server *server = client->getServer();
	string reason;
	string channel;

	if (args.size() > 1)
	{
		channel = args.front();
		args.pop_front();
		reason = args.front();
	}

	std::list<string> channels;
	channels.push_back(channel);
	
	for (std::list<string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		Channel *channel = server->getChannel(*it);
		if (channel == NULL)
		{
			client->response(":" + client->getHostname() + " 403 " + *it + " :No such channel\r\n");
			continue;
		}

		if (!channel->isOnChannel(client->getNickname()))
		{
			client->response(":" + client->getHostname() + " 442 " + *it + " :You're not on that channel\r\n");
			continue;
		}

		std::vector<Client *> members = channel->getMembers();
		for (std::vector<Client *>::iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt)
			(*memberIt)->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + *it + " :" + reason + "\r\n");

		channel->removeMember(client);
	}
}