#include "../../includes/Nick.hpp"
#include "../../includes/Server.hpp"

Nick::Nick()
{
}

Nick::~Nick()
{
}

void Nick::execute(Client *client, std::list<string> args)
{

	cout << RED << args.size() << RESET << endl;
	if (client->getPassword().empty())
	{
		ERR_PASSREQUIRED(client);
		return ;
	}

	if (args.size() == 0)
	{
		ERR_NONICKNAMEGIVEN(client);
		return ;
	}

	if (args.size() > 1)
	{
		string invalid_nick;
		while (!args.empty())
		{
			invalid_nick = invalid_nick + " " + args.front();
			args.pop_front();
		}
		ERR_ERRONEUSNICKNAME(client, invalid_nick);
		return ;
	}
	
	string nickname = args.front();
	string::iterator str_it = nickname.begin();

	for (; str_it != nickname.end(); ++str_it)
	{
		if (std::isdigit(nickname[0]) || *str_it == '#' || *str_it == ':' || *str_it == '&')
		{
			ERR_ERRONEUSNICKNAME(client, nickname);
			return;
		}
	}

	Server *server = client->getServer();
	std::map<int, Client *> &clients = server->getClients();
	std::map<int, Client *>::iterator it = clients.begin();
	
	for (; it != clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
		{
			ERR_NICKNAMEINUSE(client, nickname);
			return ;
		}
	}

	client->setNickname(nickname);
	return ;
}