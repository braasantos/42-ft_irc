#include "../../includes/User.hpp"

User::User()
{}

User::~User()
{}

void User::execute(Client* client, std::list<string> args)
{

	if (client->isAuthenticated())
	{
		ERR_ALREADYREGISTERED(client);
		return ;
	}

	if (client->getPassword().empty() || client->getNickname().empty())
	{
		ERR_NICKREQUIRED(client);
		return ;
	}
 
	if (args.size() < 4)
	{
		ERR_NEEDMOREPARAMS(client, "USER");
		return ;
	}

	string username = args.front();
	args.pop_front();
	
	string second_parameter = args.front();
	args.pop_front();
	
	string third_parameter = args.front();
	args.pop_front();
	
	string realname;


	while (!args.empty())
	{
		realname = realname + args.front();
		args.pop_front();
		if (!args.empty())
			realname = realname + " ";
	}

	if ((second_parameter.size() != 1 && second_parameter[0] != '0')
		|| (third_parameter.size() != 1 && third_parameter[0] != '*'))
	{
		ERR_INVALIDPARAMETERS(client);
		return ;
	}

	if (static_cast<int>(username.size()) > client->getUserLen())
	{
		string username_truncated;
		username_truncated = username.substr(0, client->getUserLen());
		client->setUsername(username_truncated);
	}
	else
		client->setUsername(username);

	if (realname[0] != ':')
	{
		ERR_INVALIDPARAMETERS(client);
		return ;
	}

	client->setRealname(realname);
	client->setAuthenticated(true);

	RPL_WELCOME(client);
	RPL_YOURHOST(client);
	RPL_CREATED(client);
	RPL_MYINFO(client);
	RPL_ISUPPORT(client);
}