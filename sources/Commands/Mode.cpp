#include "../../includes/Mode.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Mode::Mode() {}

Mode::~Mode() {}

void Mode::execute(Client *client, std::list<string> args)
{

	if (!client->isAuthenticated())
	{
		ERR_AUTH(client);
		return;
	}

	if (args.size() < 1)
	{
		ERR_NEEDMOREPARAMS(client, "MODE");
		return;
	}
	string modeString;
	string target = args.front();
	cout << "target " << target << endl;
	args.pop_front();
	Server *server = client->getServer();
	if (target[0] == '#' || target[0] == '&')
	{
		Channel *channel = server->getChannel(target);
		if (channel == NULL)
		{
			ERR_NOSUCHCHANNEL(client, target);
			return;
		}
		if (args.empty())
		{
			client->response(":server 324 " + target + " +" + channel->getMode() + "\r\n");
			return;
		}
		if (!args.empty())
		{
			modeString = args.front();
			args.pop_front();
			cout << "mode string " << modeString << endl;
		}
		if (!channel->isOperator(client))
		{
			client->response(":server 482 " + client->getNickname() + " " + target + " :You're not channel operator\r\n");
			return;
		}
		bool adding = false;
		for (size_t i = 0; i < modeString.size(); i++)
		{
			string nickname;
			const std::map<int, Client *> *clients = NULL;
			Client *targetClient = NULL;
			char mode = modeString[i];
			if (mode == '+')
				adding = true;
			else if (mode == '-')
				adding = false;
			else
			{
				switch (mode)
				{
				case 'i':
					if (adding)
						channel->setMode('i');
					else
						channel->unsetMode('x');
					break;
				case 't':
					if (adding)
						channel->setMode('t');
					else
						channel->unsetMode('x');
					break;
				case 'k':
					if (adding)
					{
						if (args.empty())
						{
							ERR_NEEDMOREPARAMS(client, "MODE");
							return;
						}
						string key = args.front();
						cout << key << endl;
						args.pop_front();
						channel->setKey(key);
					}
					else
						channel->unsetKey();
					break;
				case 'o':
					if (args.empty())
					{
						ERR_NEEDMOREPARAMS(client, "MODE");
						return;
					}
					nickname = args.front();
					args.pop_front();
					clients = &server->getClients();
					targetClient = NULL;
					for (std::map<int, Client *>::const_iterator it = clients->begin(); it != clients->end(); ++it)
					{
						if (it->second->getNickname() == nickname)
						{
							targetClient = it->second;
							cout << targetClient->getNickname() << endl;
							break;
						}
					}
					if (targetClient == NULL)
					{
						ERR_NOSUCHNICK(client, nickname);
						return;
					}
					if (adding)
						channel->addOperator(targetClient);
					else
						channel->removeOperator(targetClient);
					break;
				case 'l':
					if (adding)
					{
						if (args.empty())
						{
							ERR_NEEDMOREPARAMS(client, "MODE");
							return;
						}
						if (args.empty())
						{
							cout << "NO ARGUMENTS FOUND FOR AVALIABLE MODE\r\n";
							return ;
						}
						std::stringstream ss(args.front());
						int limit;
						ss >> limit;
						if (ss.fail())
						{
							client->response(":server 461 MODE " + client->getNickname() + " :Invalid parameter\r\n");
							return;
						}
						args.pop_front();
						channel->setUserLimit(limit);
					}
					else
						channel->unsetUserLimit();
					break;
				default:
					client->response(":server 501 " + client->getNickname() + " " + target + " :Unknown mode flag\r\n");
					break;
				}
			}
		}
	}
}
