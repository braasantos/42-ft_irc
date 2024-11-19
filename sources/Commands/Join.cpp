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
    }
    else
    {
        if (channel->hasKey())
        {
            if (args.empty() || args.front() != channel->getKey())
            {
                ERR_BADCHANNELKEY(client, ch_name);
                return;
            }
            args.pop_front();
        }

        if (channel->isInviteOnly() && !client->getInvited())
        {
            ERR_INVITEONLYCHAN(client, ch_name);
            return;
        }

        std::map<string, Client *> banlist = channel->getBanlist();
        if (banlist.find(client->getNickname()) != banlist.end())
        {
            ERR_BANNEDFROMCHAN(client, ch_name);
            return;
        }

        if (channel->getMode() == 'l' && static_cast<int>(channel->getMembers().size()) >= channel->getUserlimit())
        {
            ERR_CHANNELUSERLIMIT(client, ch_name);
            return;
        }

        if (channel->isOnChannel(client->getNickname()))
        {
            ERR_ALREADYJOINED(client, ch_name);
            return;
        }

		if (channel->getMembers().size() == static_cast<unsigned int>(channel->getUserlimit()))
		{
			ERR_CHANNELUSERLIMIT(client, ch_name);
			return;
		}
        channel->addMember(client);
    }

    std::vector<Client *> members = channel->getMembers();
    for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
    {
        Client *member = *it;
        member->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN " + ch_name + "\r\n");
    }

    std::string names;
    for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (channel->isOperator(*it))
            names += "@" + (*it)->getNickname() + " ";
        else
            names += "+" + (*it)->getNickname() + " ";
    }

    for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
    {
        Client *member = *it;
        member->response(":server 353 " + member->getNickname() + " = " + ch_name + " :" + names + "\r\n");
        member->response(":server 366 " + member->getNickname() + " " + ch_name + " :End of /NAMES list\r\n");
    }

    if (channel->getTopic().empty())
    {
        RPL_NOTOPIC(client, ch_name);
    }
    else
        RPL_TOPIC(client, channel);
}