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
		return;
	}

	string ch_name = args.front();
	args.pop_front();
	if (ch_name[0] != '#')
	{
		ERR_NOSUCHCHANNEL(client, ch_name);
		return;
	}

	std::vector<Channel *> client_channels = client->getInvitedChannels();
	if (client_channels.size() == client->getChannelLimit())
	{
		ERR_TOOMANYCHANNELS(client, ch_name);
		return;
	}

	Server *server = client->getServer();
	Channel *channel = server->getChannel(ch_name);

	if (channel == NULL)
	{
		channel = new Channel();
		server->addChannel(ch_name, channel);
		channel->addMember(client);
		channel->addOperator(client);
		client->addInvitedChannel(channel);
		client->response(":" + client->getNickname() + " JOIN " + ch_name + "\r\n");
		client->response(":" + server->getServerName() + " 331 " + client->getNickname() + " " + ch_name + " :No topic is set\r\n");
	}
	if (channel->hasKey())
	{
		if (args.empty())
		{
			ERR_NEEDMOREPARAMS(client, "JOIN");
			return ;
		}
		else
		{
			string key = args.front();
			cout << key << endl;
			if (key.empty())
			{
				cout << "ERROR\r\n";
				return;
			}
			if (key != channel->getKey())
			{
				cout << "ERROR\r\n";
				return;
			}
		}
	}
	if (channel->isInviteOnly())
	{
		if (!client->getInvited())
		{
			cout << "INVITE ONLY\r\n";
			// ERR_MODEINVITEONLY(client, ch_name);
			return ;
		}
	}
	std::map<string, Client *> banlist = channel->getBanlist();
	std::map<string, Client*>::iterator it = banlist.begin();

	for (; it != banlist.end(); ++it)
	{
		if (it->first == client->getNickname())
		{
			ERR_INVITEONLYCHAN(client, ch_name);
			return;
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

	for(std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
	{
		if (*it == client)
		{
			ERR_ALREADYJOINED(client, ch_name);
			return;
		}

		if (channel->addMember(client))
		{
			ERR_CHANNELUSERLIMIT(client, ch_name);
			return;
		}

		client->response(":" + client->getNickname() + " JOIN " + ch_name + "\r\n");

		if (channel->getTopic().empty())
			client->response(":" + server->getServerName() + " 331 " + client->getNickname() + " " + ch_name + " :No topic is set\r\n");
		else
			client->response(":" + server->getServerName() + " 332 " + client->getNickname() + " " + ch_name + " :" + channel->getTopic() + "\r\n");
	}
	// std::vector<Channel *>::iterator ch_it = client_channels.begin();
	// for (; ch_it != client_channels.end(); ++ch_it)
	// {
	// 	if (*ch_it == channel)
	// 	{
	// 		ERR_ALREADYJOINED(client, ch_name);
	// 		return;
	// 	}
	// }



	// std::vector<Client *> members = channel->getMembers();
	string names;
	for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
	{
		if (channel->isOperator(*it))
			names += "@" + (*it)->getNickname() + " ";
		else
			names += "+" + (*it)->getNickname() + " ";
	}

	client->response(":" + server->getServerName() + " 353 " + client->getNickname() + " = " + ch_name + " :" + names + "\r\n");
	client->response(":" + server->getServerName() + " 366 " + client->getNickname() + " " + ch_name + " :End of /NAMES list\r\n");

	client->addInvitedChannel(channel);
}
