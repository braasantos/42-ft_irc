#include "../../includes/Join.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"

Join::Join() {}

Join::~Join() {}

void Join::execute(Client *client, std::list<string> args)
{

	if (!client->isAuthenticated())
	{
		ERR_AUTH(client);
		return;
	}

	if (args.empty())
	{
		ERR_NEEDMOREPARAMS(client, "JOIN");
		return ;
	}

	string ch_name = args.front();

	if (ch_name[0] != '#')
	{
		ERR_NOSUCHCHANNEL(client, ch_name);
		return ;
	}

	std::vector<Channel *> client_channels = client->getInvitedChannels();

	if (client_channels.size() == client->getChannelLimit())
	{
		ERR_TOOMANYCHANNELS(client, ch_name);
		return ;
	}

	Server *server = client->getServer();
	Channel *channel = server->getChannel(ch_name);

	if (channel == NULL)
	{
		Channel *newChannel = new Channel();
		std::vector<Client *> members = newChannel->getMembers();
		std::vector<Client *>::iterator names_it = members.begin();
		string names;

		newChannel->addMember(client);
		newChannel->addOperator(client);
		std::vector<Channel *>invited_channels = client->getInvitedChannels();
		client->addInvitedChannel(newChannel);
		server->addChannel(ch_name, newChannel);

		client->response(":" + client->getNickname() + " JOIN " + ch_name + "\r\n");
		
		if (newChannel->getTopic().empty())
			client->response(client->getNickname() + " " + ch_name + " :No topic is set\r\n");
		else
			client->response(client->getNickname() + " " + ch_name + " :" + newChannel->getTopic() + "\r\n");

		for (; names_it != members.end(); ++names_it)
			names += (*names_it)->getNickname() + " ";
		
		names += client->getNickname();
		
		client->response(client->getNickname() + " -> " + ch_name + " :" + names + "\r\n");
		client->response(client->getNickname() + " " + ch_name + ":End of NAMES list\r\n");
		
		return ;
	}

	if (channel->isInviteOnly())
	{
		ERR_INVITEONLYCHAN(client, ch_name);
		return ;
	}

	std::map<string, Client *> banlist = channel->getBanlist();
	std::map<string, Client*>::iterator it = banlist.begin();

	for (; it != banlist.end(); ++it)
	{
		if (it->first == client->getNickname())
		{
			ERR_BANNEDFROMCHAN(client, ch_name);
			return ;
		}
	}

	std::vector<Client *> members = channel->getMembers();

	if (channel->getMode() == 'l')
	{
		if (static_cast<int>(members.size()) == channel->getUserlimit())
		{
			ERR_CHANNELUSERLIMIT(client, ch_name);
			return ;
		}
	}

	std::vector<Channel *>::iterator ch_it = client_channels.begin();
	for (; ch_it != client_channels.end(); ++ch_it)
	{
		if (*ch_it == channel)
		{
			ERR_ALREADYJOINED(client, ch_name);
			return;
		}
	}

	if (channel->addMember(client))
		return ;
	client->response(":" + client->getNickname() + " JOIN " + ch_name + "\r\n");

	if (channel->getTopic().empty())
		client->response(client->getNickname() + " " + ch_name + " :No topic is set\r\n");
	else
		client->response(client->getNickname() + " " + ch_name + " :" + channel->getTopic() + "\r\n");

	string names;
	std::vector<Client *>::iterator names_it = members.begin();

	for (; names_it != members.end(); ++names_it)
		names += (*names_it)->getNickname() + " ";
	
	names += client->getNickname();
	client->response(client->getNickname() + " -> " + ch_name + " :" + names + "\r\n");
	client->response(client->getNickname() + " " + ch_name + ":End of /NAMES list\r\n");

	client->addInvitedChannel(channel);
	return ;
}
