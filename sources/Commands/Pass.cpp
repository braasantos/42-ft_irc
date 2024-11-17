#include "../../includes/Pass.hpp"

Pass::Pass()
{
}

Pass::~Pass()
{
}

void Pass::execute(Client *client, std::list<string> args)
{

	if (client->isAuthenticated())
	{
		ERR_ALREADYREGISTERED(client);
		return;
	}

	if (args.size() == 0)
	{
		ERR_NEEDMOREPARAMS(client, "PASS");
		return;
	}

	string password = args.front();
	Server *server = client->getServer();

	if (password != client->getServer()->getPassword())
	{
		ERR_PASSWDMISMATCH(client);
		ERROR(client);
		close(client->getFd());
		server->removeClient(client->getFd());
		return;
	}

	client->setPassword(password);
	return;
}