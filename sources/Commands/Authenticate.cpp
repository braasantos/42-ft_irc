#include "../includes/Authenticate.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include "../includes/Server.hpp"

Authenticate::Authenticate()
{
}

Authenticate::~Authenticate()
{
}

void Authenticate::execute(Client *client, std::list<string> args)
{
	(void)args;

	if (client->isAuthenticated())
	{
		ERR_ALREADYREGISTERED(client);
		return;
	}

	Server *server = client->getServer();

	if (!client->getPassword().empty() && !client->getNickname().empty() && !client->getUsername().empty())
	{
		RPL_WELCOME(client);
		RPL_YOURHOST(client);
		RPL_CREATED(client, server);
		RPL_MYINFO(client);
		RPL_ISUPPORT(client);
		RPL_INFO(client);
		RPL_MOTDSTART(client);
		client->response(":server 372 " + client->getNickname() + " ██ ███    ██ ████████ ███████ ██████  ███    ██ ███████ ████████     ██████  ███████ ██       █████  ██    ██      ██████ ██   ██  █████  ████████\r\n");
		client->response(":server 372 " + client->getNickname() + " ██ ████   ██    ██    ██      ██   ██ ████   ██ ██         ██        ██   ██ ██      ██      ██   ██  ██  ██      ██      ██   ██ ██   ██    ██   \r\n");
		client->response(":server 372 " + client->getNickname() + " ██ ██ ██  ██    ██    █████   ██████  ██ ██  ██ █████      ██        ██████  █████   ██      ███████   ████       ██      ███████ ███████    ██   \r\n");
		client->response(":server 372 " + client->getNickname() + " ██ ██  ██ ██    ██    ██      ██   ██ ██  ██ ██ ██         ██        ██   ██ ██      ██      ██   ██    ██        ██      ██   ██ ██   ██    ██   \r\n");
		client->response(":server 372 " + client->getNickname() + " ██ ██   ████    ██    ███████ ██   ██ ██   ████ ███████    ██        ██   ██ ███████ ███████ ██   ██    ██         ██████ ██   ██ ██   ██    ██   \r\n");
		client->response(":server 376 " + client->getNickname() + " :End of message of the day.\r\n");
		client->response(":server 374 " + client->getNickname() + " :Now you can enjoy the chat =).\r\n");
		client->setAuthenticated(true);
		return ;
	}
	else
	{
		ERR_NICKREQUIRED(client);
		return ;
	}
}