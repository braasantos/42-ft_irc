#include "../../includes/Kick.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"

Kick::Kick()
{}

Kick::~Kick()
{}

void Kick::execute(Client* client, std::list<string> args)
{
	if (args.empty())
	{
		ERR_NORECIPIENT(client, "PRIVMSG");
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
		reason += args.front();
		args.pop_front();
	}
	else
	{
		reason = "No specific reson";
	}

	Server *server = client->getServer();
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
	Channel *channel = server->getChannel(currChannel);
	if (!channel->isOperator(client))
	{
		cout << client->getNickname() << endl;
		ERR_CANNOTREMOVEUSER(client, targetToKick)
		return ;
	}
	std::vector<Client *> members = channel->getMembers();
	for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); ++it)
	{
        Client* client = *it;
		if (client->getNickname() == targetToKick)
		{
			channel->removeMember(*it);
			cout << targetToKick + " was removed " + "for " + reason + " by " + client->getNickname() <<endl;
			return ;
		}
    }
}