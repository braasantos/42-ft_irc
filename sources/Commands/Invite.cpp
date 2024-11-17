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
	if (args.size() < 2)
	{
		client->response("461 INVITE :Not enough parameters\r\n");
		return;
	}
	std::string chName = args.front();
	cout << chName << endl;
	args.pop_front();
	std::string nickname = args.front();
	string newNick = nickname.substr(1);
	cout << newNick << endl;
	cout << nickname << endl;
	Server *server = client->getServer();
	Channel *channel = server->getChannel(chName);
	args.pop_front();
	if (chName[0] != '#' && chName[0] != '&')
	{
		client->response("403 " + chName + " :No such chName\r\n");
		return;
	}
	if (nickname[0] != ':' && nickname[0] != '&')
	{
		client->response("401 " + nickname + " :No such nick/chName\r\n");
		return;
	}
	if (channel == NULL)
	{
		client->response("403 " + chName + " :No such channel\r\n");
		return;
	}
	if (channel->isOnChannel(nickname) )
	{
		cout << "already on channel" << endl;
		return ;

	}
	if (!channel->isOnChannel(client->getNickname()))
	{
		cout << "Not on channel" << endl;
		return ;
	}
	std::map<int, Client*> target = server->getClients();
	for (std::map<int, Client*>::iterator it = target.begin(); it != target.end(); ++it)
	{
		if (it->second->getNickname() == newNick )
		{
			cout << it->second->getNickname() << endl;
			it->second->setInvited(true);
			return ;
		}
    }
	// if (client->getChannel(channel)->getMode() != 'i' && !client->getChannel(channel)->isOperator(client))
	// {
	// 	client->response("482 " + channel + " :You're not channel operator\r\n");
	// 	return;
	// }
	// if (client->getChannel(channel)->isOnChannel(nickname))
	// {
	// 	client->response("443 " + nickname + " " + channel + " :is already on channel\r\n");
	// 	return;
	// }
	// client->getChannel(channel)->addMember(client);
	// client->response("341 " + nickname + " " + channel + " :Inviting you on channel\r\n");
	// client->response("INVITE " + nickname + " " + channel + "\r\n");
}