#include "../../includes/Ping.hpp"

Ping::Ping(){}

Ping::~Ping(){}

void Ping::execute(Client *client, std::list<string> args)
{

    Server *server = client->getServer();

    if (args.empty())
    {
        ERR_NOORIGIN(client);
        return ;
    }

    string origin = args.front();
    args.pop_front();

    client->response(":server PONG " + server->getServerName() + " " + origin + "\r\n");
    return ;
}