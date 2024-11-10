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
	// Server *server = client->getServer();
	string nickname = args.front();
	// const std::map<int, Client *> &clients = server->getClients();

	if (args.size() == 0)
	{
		ERR_NONICKNAMEGIVEN(client);
		return;
	}

	while (!args.empty())
	{
		cout << args.front() << endl;
		args.pop_front();
	}
}