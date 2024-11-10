#include "../../includes/Quit.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void Quit::execute(Client *client, std::list<string> args)
{
	Server *server = client->getServer();
	string reason = "Bye for now!";
	if (!args.empty())
		reason = args.front();

	for (std::map<string, Channel *>::iterator it = server->getChannels().begin(); it != server->getChannels().end(); ++it)
	{
		Channel *channel = it->second;
		if (channel->isOnChannel(client->getNickname()))
		{
			std::vector<Client *> members = channel->getMembers();
			for (std::vector<Client *>::iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt)
			{
				(*memberIt)->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " QUIT :" + reason + "\r\n");
			}
			channel->removeMember(client);
		}
	}

	close(client->getFd());
	server->removeClient(client->getFd());
}