#include "../../includes/Invite.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

Invite::Invite()
{}

Invite::~Invite()
{}

void Invite::execute(Client* client, std::list<string> args)
{

	if (!client->isAuthenticated())
	{
		ERR_AUTH(client);
		return;
	}

	if (args.size() < 2)
	{
		ERR_NEEDMOREPARAMS(client, "INVITE");
		return;
	}
	std::string chName = args.front();
	args.pop_front();
	std::string nickname = args.front();
	string newNick = nickname.substr(1);
	Server *server = client->getServer();
	Channel *channel = server->getChannel(chName);
	args.pop_front();
	if (chName[0] != '#' && chName[0] != '&')
	{
		ERR_NOSUCHCHANNEL(client, chName);
		return;
	}
	if (nickname[0] != ':' && nickname[0] != '&')
	{
		ERR_NOSUCHNICK(client, nickname);
		return;
	}
	if (channel == NULL)
	{
		ERR_NOSUCHCHANNEL(client, chName);
		return;
	}
	if (channel->isOnChannel(nickname) )
	{
		ERR_NOSUCHNICKONCH(client, nickname);
		return ;

	}
	if (!channel->isOnChannel(client->getNickname()))
	{
		ERR_NOSUCHNICKONCH(client, client-<getNickname());
		return ;
	}
	std::map<int, Client*> target = server->getClients();
	for (std::map<int, Client*>::iterator it = target.begin(); it != target.end(); ++it)
	{
		if (it->second->getNickname() == newNick )
		{
			it->second->setInvited(true);
			return ;
		}
    }
}