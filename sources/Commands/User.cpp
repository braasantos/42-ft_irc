#include "../../includes/User.hpp"

User::User()
{}

User::~User()
{}

void User::execute(Client* client, std::list<string> args)
{

	/**
	 * TODO: User command only can work while client was registering, after that it should throw an error
	 * TODO: "<client> :You may not reregister"
	 * TODO: Returned when a client tries to change a detail that can only be set during registration
	 * TODO: (such as resending the PASS or USER after registration).
	 */

	if (args.size() < 4)
	{
		cout << RED << "Not enough arguments for USER command" << RESET << endl;
		ERR_NEEDMOREPARAMS(client, "USER");
		return;
	}

	client->setUsername(args.front());

	for(int i = 0; i < 3; i++)
		args.pop_front();

	string helper = args.front();
	string realname = helper.substr(1, helper.size() - 1);
	args.pop_front();
	if (!args.empty())
		realname += " " + args.front();
	client->setRealname(realname);
	cout << PURPLE << "Client Realname = " << client->getRealname() << RESET << endl;
}