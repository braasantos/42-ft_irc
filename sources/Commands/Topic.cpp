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
		ERR_AUTH(client);
		return;
	}

	(void)client;
	(void)args;
}