#include "../../includes/CommandHandler.hpp"
#include "../../includes/Client.hpp"

CommandHandler::CommandHandler()
{
	_commands["INVITE"] = new Invite();
	_commands["JOIN"] = new Join();
	_commands["KICK"] = new Kick();
	_commands["MODE"] = new Mode();
	_commands["NICK"] = new Nick();
	_commands["PART"] = new Part();
	_commands["PASS"] = new Pass();
	_commands["PRIVMSG"] = new Privmsg();
	_commands["QUIT"] = new Quit();
	_commands["TOPIC"] = new Topic();
	_commands["USER"] = new User();
}

CommandHandler::~CommandHandler()
{
}

void CommandHandler::handleCommand(string command, Client *client)
{
	if (command[0] == ':')
		command = command.substr(1);

	std::stringstream ss(command);
	string cmd;
	while (std::getline(ss, cmd))
	{
		string command_name = cmd.substr(0, cmd.find(" "));

		cout << YELLOW << "Command: " << command_name << RESET << endl;

		std::map<string, Command *>::iterator it = _commands.find(command_name);
		if (command_name == "CAP")
			continue;
		else if (it == _commands.end())
		{
			cout << RED << "Command not found" << RESET << endl;
			continue;
		}

		try
		{
			Command *operation = it->second;

			string argsBuffer = cmd.substr(cmd.find(" ") + 1);
			std::istringstream argsStream(argsBuffer);
			string argument;
			std::list<string> args;
			while (std::getline(argsStream, argument, ' '))
			{
				argument.erase(std::remove_if(argument.begin(), argument.end(), ::isspace), argument.end());
				args.push_back(argument);
			}
			// TODO: Authentication check.
			// TODO: Client need to follow the steps

			// TODO: 1st set a PASS
			// TODO: 2st set a NICKNAME
			// TODO: 3st set a USERNAME

			// TODO: after that this instructions
			// TODO: will be blocked to be called by the execute command.

			if (client->isAuthenticated() == true && (command_name == "PASS"
				|| command_name == "NICK" || command_name == "USER"))
				{
					ERR_ALREADYREGISTERED(client);
					break;
				}

			if (client->isAuthenticated() == true)
			{
				operation->execute(client, args);
				break ;
			}
			
			if (client->getPassword().empty() && command_name == "PASS")
			{
				// todo: check all the informations and errors inside the password command
				operation->execute(client, args);
				break ;
			}

			if (!client->getPassword().empty()
				&& client->getNickname().empty()
				&& command_name == "NICK")
			{
				// todo: check all the informations and errors inside the NICK command
				operation->execute(client, args);
				break ;
			}

			if (!client->getPassword().empty()
				&& client->getNickname().empty()
				&& client->getUsername().empty()
				&& command_name == "USER")
			{
				operation->execute(client, args);
				break ;
			}


		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			throw std::runtime_error("Error: Cannot handle command");
		}
	}
}
