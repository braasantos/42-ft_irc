#include "../../includes/Pass.hpp"

Pass::Pass()
{}

Pass::~Pass()
{}

void Pass::execute(Client* client, std::list<string> args)
{
	string password = args.front();
	Server* server = new Server();

	if (password != client->getServer()->getPassword())
	{
		client->response("Password incorrect\r\n");
		ERROR();
		close(client->getFd());
		server->removeClient(client->getFd());
		return;
	}
	client->authenticatePassword(true);
	client->setPassword(password);
	client->response("Password accepted\r\n");
}