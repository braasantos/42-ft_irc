#include "../../includes/Quit.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Quit::Quit() {}

Quit::~Quit() {}

void Quit::execute(Client *client, std::list<string> args)
{
	Server *server = client->getServer();
	string reason;
	if (args.empty())
		reason = "Bye for now!";
	if (!args.empty())
	{
		while (!args.empty())
		{
			reason += args.front();
			args.pop_front();
			if (!args.empty())
				reason += " ";
		}
	}

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
	std::vector<pollfd> &fd_poll = server->getPollFd();
    std::vector<pollfd>::iterator poll_it = fd_poll.begin();
    pollfd client_poll = {client->getFd(), POLLIN, 0};
    for(; poll_it != fd_poll.end(); ++poll_it)
    {
        if (poll_it->fd == client_poll.fd)
        {
            fd_poll.erase(poll_it);
            break;
        }
    }
	close(client->getFd());
	server->removeClient(client->getFd());
}