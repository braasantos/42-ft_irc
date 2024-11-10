#include "../../includes/Join.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

Join::Join() {}

Join::~Join() {}

void Join::execute(Client *client, std::list<string> args)
{
	if (args.empty())
	{
		client->response(":" + client->getHostname() + " 461 JOIN :Not enough parameters\r\n");
		return;
	}

	Server *server = client->getServer();
	string ch_name = args.front();
	Channel *ch_helper = server->getChannel(ch_name);

	if (ch_name[0] != '#')
	{
		client->response(":" + client->getHostname() + " 403 " + client->getNickname() + " " + ch_name + " :No such channel\r\n");
		return;
	}

	if (ch_helper == NULL)
	{
		Channel *channel = new Channel();
		server->addChannel(ch_name, channel);
		client->response(":" + client->getNickname() + " JOIN " + ch_name + "\r\n");
		ch_helper = channel;
		server->addChannel(ch_name, ch_helper);
	}
	else
		client->response(":" + client->getNickname() + " JOIN " + ch_name + "\r\n");

	if (!ch_helper->getTopic().empty())
		client->response("332 " + client->getNickname() + " " + ch_name + " :" + ch_helper->getTopic() + "\r\n");
	else
		client->response("331 " + client->getNickname() + " " + ch_name + " :No topic is set\r\n");

	std::vector<Client *> members = ch_helper->getMembers();
	string names;
	for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
		names += (*it)->getNickname() + " ";

	names += client->getNickname();
	client->response("353 " + client->getNickname() + " -> " + ch_name + " :" + names + "\r\n");
	client->response("366 " + client->getNickname() + " " + ch_name + " :End of /NAMES list\r\n");

	ch_helper->addMember(client);

}