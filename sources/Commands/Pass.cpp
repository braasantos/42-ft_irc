#include "../../includes/Pass.hpp"

Pass::Pass()
{}

Pass::~Pass()
{}

void Pass::execute(Client* client, std::list<string> args)
{
	/**
	 * !The PASS command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made.
	 * !This requires that clients send a PASS command before sending the NICK / USER combination.
	 * !The password supplied must match the one defined in the server configuration.
	 * !It is possible to send multiple PASS commands before registering
	 * !but only the last one sent is used for verification and it may not be changed once the client has been registered.
	 * !If the password supplied does not match the password expected by the server, then the server SHOULD send ERR_PASSWDMISMATCH (464)
	 * !and MAY then close the connection with ERROR. Servers MUST send at least one of these two messages.
	 * !Servers may also consider requiring SASL authentication upon connection as an alternative to this
	 * !when more information or an alternate form of identity verification is desired.
	 */

	// !Numeric replies:
    //!ERR_NEEDMOREPARAMS (461)
    //!ERR_ALREADYREGISTERED (462)
    //!ERR_PASSWDMISMATCH (464)


	if (client->isAuthenticated())
	{
		ERR_ALREADYREGISTERED(client);
		return ;
	}

	if (args.size() == 0)
	{
		ERR_NEEDMOREPARAMS(client, "PASS");
		return ;
	}

	string password = args.front();
	Server* server = client->getServer();

	if (password != client->getServer()->getPassword())
	{
		ERR_PASSWDMISMATCH(client);
		ERROR(client);
		close(client->getFd());
		server->removeClient(client->getFd());
		return;
	}

	client->setPassword(password);
	client->response("Password accepted\r\n");
	return ;
}