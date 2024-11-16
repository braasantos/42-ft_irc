#include "../../includes/Invite.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Invite::Invite()
{}

Invite::~Invite()
{}

void Invite::execute(Client* client, std::list<string> args)
{

	if (!client->isAuthenticated())
	{
		client->response(":server 451 INVITE :You have not registered\r\n");
		return;
	}

	if (args.size() < 2)
	{
		client->response("461 INVITE :Not enough parameters\r\n");
		return;
	}
	std::string channel = args.front();
	args.pop_front();
	std::string nickname = args.front();
	args.pop_front();
	if (channel[0] != '#' && channel[0] != '&')
	{
		client->response("403 " + channel + " :No such channel\r\n");
		return;
	}
	if (nickname[0] != ':' && nickname[0] != '&')
	{
		client->response("401 " + nickname + " :No such nick/channel\r\n");
		return;
	}
	if (client->getChannel(channel) == NULL)
	{
		client->response("403 " + channel + " :No such channel\r\n");
		return;
	}
	if (client->getChannel(channel)->getMode() != 'i' && !client->getChannel(channel)->isOperator(client))
	{
		client->response("482 " + channel + " :You're not channel operator\r\n");
		return;
	}
	if (client->getChannel(channel)->isOnChannel(nickname))
	{
		client->response("443 " + nickname + " " + channel + " :is already on channel\r\n");
		return;
	}
	client->getChannel(channel)->addMember(client);
	client->response("341 " + nickname + " " + channel + " :Inviting you on channel\r\n");
	client->response("INVITE " + nickname + " " + channel + "\r\n");
}