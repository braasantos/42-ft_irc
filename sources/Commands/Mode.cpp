#include "../../includes/Mode.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"

Mode::Mode() {}

Mode::~Mode() {}

void Mode::execute(Client *client, std::list<string> args)
{
	if (args.size() < 1)
	{
		client->response(":" + client->getHostname() + " 461 MODE :Not enough parameters\r\n");
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
			client->response(":" + client->getHostname() + " 403 " + target + " :No such channel\r\n");
			return;
		}
		if (args.empty())
		{
			client->response(":" + client->getHostname() + " 324 " + target + " +" + channel->getMode() + "\r\n");
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
			client->response(":" + client->getHostname() + " 482 " + target + " :You're not channel operator\r\n");
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
						channel->unsetMode('i');
					break;
				// case 't':
				// 	if (adding)
				// 		channel->setMode('t');
				// 	else
				// 		channel->unsetMode('t');
				// 	break;
				case 'k':
					if (adding)
					{
						if (args.empty())
						{
							client->response(":" + client->getHostname() + " 461 MODE :Not enough parameters\r\n");
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
						client->response(":" + client->getHostname() + " 461 MODE :Not enough parameters\r\n");
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
						client->response(":" + client->getHostname() + " 401 " + nickname + " :No such nick/channel\r\n");
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
							client->response(":" + client->getHostname() + " 461 MODE :Not enough parameters\r\n");
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
							client->response(":" + client->getHostname() + " 461 MODE :Invalid parameter\r\n");
							return;
						}
						args.pop_front();
						channel->setUserLimit(limit);
					}
					else
						channel->unsetUserLimit();
					break;
				default:
					client->response(":" + client->getHostname() + " 501 " + target + " :Unknown mode flag\r\n");
					break;
				}
			}
		}
	}
}
	// 	string nickname;
	// 	const std::map<int, Client *> *clients = NULL;
	// 	Client *targetClient = NULL;
	// 	for (size_t i = 0; i < modeString.size(); ++i)
	// 	{
	// 		char mode = modeString[i];
	// 		if (mode == '+')
	// 			adding = true;
	// 		else if (mode == '-')
	// 			adding = false;
	// 		else
	// 		{
	// 			switch (mode)
	// 			{
	// 			case 'i':
	// 				if (adding)
	// 					channel->setMode('i');
	// 				else
	// 					channel->unsetMode('i');
	// 				break;
	// 			case 't':
	// 				if (adding)
	// 					channel->setMode('t');
	// 				else
	// 					channel->unsetMode('t');
	// 				break;
	// 			case 'k':
	// 				if (adding)
	// 				{
	// 					if (args.empty())
	// 					{
	// 						client->response(":" + client->getHostname() + " 461 MODE :Not enough parameters\r\n");
	// 						return;
	// 					}
	// 					string key = args.front();
	// 					args.pop_front();
	// 					channel->setKey(key);
	// 				}
	// 				else
	// 					channel->unsetKey();
	// 				break;
	// 			case 'o':
	// 				if (args.empty())
	// 				{
	// 					client->response(":" + client->getHostname() + " 461 MODE :Not enough parameters\r\n");
	// 					return;
	// 				}
	// 				nickname = args.front();
	// 				args.pop_front();
	// 				clients = &server->getClients();
	// 				targetClient = NULL;
	// 				for (std::map<int, Client *>::const_iterator it = clients->begin(); it != clients->end(); ++it)
	// 				{
	// 					if (it->second->getNickname() == nickname)
	// 					{
	// 						targetClient = it->second;
	// 						break;
	// 					}
	// 				}
	// 				if (targetClient == NULL)
	// 				{
	// 					client->response(":" + client->getHostname() + " 401 " + nickname + " :No such nick/channel\r\n");
	// 					return;
	// 				}
	// 				if (adding)
	// 					channel->addOperator(targetClient);
	// 				else
	// 					channel->removeOperator(targetClient);
	// 				break;
	// 			case 'l':
	// 				if (adding)
	// 				{
	// 					if (args.empty())
	// 					{
	// 						client->response(":" + client->getHostname() + " 461 MODE :Not enough parameters\r\n");
	// 						return;
	// 					}
	// 					std::stringstream ss(args.front());
	// 					int limit;
	// 					ss >> limit;
	// 					if (ss.fail())
	// 					{
	// 						client->response(":" + client->getHostname() + " 461 MODE :Invalid parameter\r\n");
	// 						return;
	// 					}
	// 					args.pop_front();
	// 					channel->setUserLimit(limit);
	// 				}
	// 				else
	// 					channel->unsetUserLimit();
	// 				break;
	// 			default:
	// 				client->response(":" + client->getHostname() + " 501 " + target + " :Unknown mode flag\r\n");
	// 				break;
	// 			}
	// 		}
	// 	}

	// 	std::vector<Client *> members = channel->getMembers();
	// 	for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
	// 		(*it)->response(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " MODE " + target + " " + modeString + "\r\n");
	// }
	// else
	// 	client->response(":" + client->getHostname() + " 501 " + target + " :Unknown mode flag\r\n"); 