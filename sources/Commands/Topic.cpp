#include "../../includes/Topic.hpp"

Topic::Topic()
{
}

Topic::~Topic()
{
}

void Topic::execute(Client *client, std::list<string> args)
{

	if (!client->isAuthenticated())
	{
		client->response(":server 451 INVITE :You have not registered\r\n");
		return;
	}

	(void)client;
	(void)args;
}