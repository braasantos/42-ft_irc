#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/CommandHandler.hpp"

/**
 * @brief Construct a new Server:: Server object
 *
 */

Server::Server()
{
	_commandHandler = new CommandHandler();
	_serverName = "42-ModernIRC";
}

/**
 * @brief Destroy the Server object
 *
 */

Server::~Server()
{
	close(this->_socketfd);

	delete _commandHandler;

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		close(it->first);
		delete it->second;
	}

	_clients.clear();
	_pollfds.clear();
	cout << GREEN << "Server stopped" << RESET << endl;
}

/**
 * @brief Create a socket for the server
 *
 * @param port
 */

void Server::createSocket(int port)
{
	this->_socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketfd == -1)
	{
		close(this->_socketfd);
		throw std::runtime_error("Error: socket creation failed");
	}
	int opt = 1;
	if (setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(this->_socketfd);
		throw std::runtime_error("Error: Cannot set socket options");
	}
	_server_sockIPV4.sin_family = AF_INET;
	_server_sockIPV4.sin_addr.s_addr = INADDR_ANY;
	_server_sockIPV4.sin_port = htons(port);

	if (fcntl(this->_socketfd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(this->_socketfd);
		throw std::runtime_error("Error: Cannot set socket to non-blocking mode");
	}

	if (bind(this->_socketfd, (struct sockaddr *)&_server_sockIPV4, sizeof(_server_sockIPV4)) == -1)
	{
		close(this->_socketfd);
		throw std::runtime_error("Error: Cannot bind socket to address");
	}

	if (listen(this->_socketfd, 5) == -1)
	{
		close(this->_socketfd);
		throw std::runtime_error("Error: Cannot listen on socket");
	}

	this->_port = port;

	cout << GREEN << "Server socket created and listening on port " << port << RESET << endl;
}

/**
 * @brief Start the server
 *
 */

void Server::startServerIPV4()
{
	pollfd serverfd = {this->_socketfd, POLLIN, 0};
	_pollfds.push_back(serverfd);
	cout << GREEN << "Server started and running" << RESET << endl;
	setServerCreatedTime();

	while (true)
	{
		if (_pollfds.empty())
			_pollfds.push_back(serverfd);

		if (poll(_pollfds.data(), _pollfds.size(), -1) == -1)
			throw std::runtime_error("Error: Poll failed");

		for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
		{
			if (it->revents == 0)
				continue;
			if (it->revents & POLLIN)
			{
				if (it->fd == this->_socketfd)
				{
					addClient(_socketfd);
					break;
				}
				else
				{
					handleMessageFromClient(it->fd);
					break;
				}
			}
		}
	}
}

/**
 * @brief Add a client to the server
 *
 * @param socketfd
 */

void Server::addClient(int socketfd)
{
	cout << PURPLE << "Adding client.." << RESET << endl;
	int client_fd;
	sockaddr_in clientAddress = {};
	socklen_t clientAddressLength = sizeof(clientAddress);

	client_fd = accept(socketfd, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (client_fd == -1)
		throw std::runtime_error("Error: Cannot accept client connection");

	pollfd client_poll = {client_fd, POLLIN, 0};
	_pollfds.push_back(client_poll);

	char hostname[NI_MAXHOST];
	int result = getnameinfo((struct sockaddr *)&clientAddress, clientAddressLength, hostname, NI_MAXHOST, NULL, 0, 0);

	if (result != 0)
	{
		close(client_fd);
		throw std::runtime_error("Error: Cannot get client hostname");
	}

	Client *client = new Client(hostname, ntohs(clientAddress.sin_port), client_fd, this);
	_clients.insert(std::make_pair(client_fd, client));
	cout << PURPLE << "Client added" << RESET << endl;
}

/**
 * @brief Handle message from client
 *
 * @param fd
 */

void Server::handleMessageFromClient(int fd)
{
	try
	{
		cout << ORANGE << "Handling message from client.." << RESET << endl;
		std::map<int, Client *>::iterator it = _clients.find(fd);
		if (it == _clients.end())
			return;

		Client *client = it->second;
		string message = readFromSocket(fd);

		cout << CYAN << "Message: " << message << RESET << endl;

		_commandHandler->handleCommand(message, client);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		throw std::runtime_error("Error: Cannot handle message from client");
	}
}

/**
 * @brief Read from socket
 *
 * @param fd
 * @return string
 */

string Server::readFromSocket(int fd)
{
	char buffer[1024];
	string message;

	int bytes_read = recv(fd, buffer, 1024, 0);
	if (bytes_read == -1)
		throw std::runtime_error("Error: Cannot read from socket");
	else
		message = string(buffer, bytes_read);

	return (message);
}

/**
 * @brief Get the Channel object
 *
 * @param channel_name
 * @return Channel*
 */

Channel *Server::getChannel(const string &channel_name)
{
	if (_channels.find(channel_name) != _channels.end())
		return (_channels[channel_name]);
	return (NULL);
}

Client *Server::getClient(const int &fd)
{
	if (_clients.find(fd) != _clients.end())
		return (_clients[fd]);
	return (NULL);
}

/**
 * @brief Add a channel to the server
 *
 * @param channel_name
 * @param channel
 */

void Server::addChannel(const string &channel_name, Channel *channel)
{
	_channels[channel_name] = channel;
}

/**
 * @brief Get the Clients object
 *
 * @return const std::map<int, Client*>&
 */

std::map<int, Client *> &Server::getClients()
{
	return _clients;
}

/**
 * @brief Remove a client from the server
 *
 * @param client_fd
 */

void Server::removeClient(int client_fd)
{
	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}

	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
	{
		if (it->fd == client_fd)
		{
			_pollfds.erase(it);
			break;
		}
	}
}

/**
 * @brief Get the Channels object
 *
 * @return const std::map<string, Channel*>&
 */
std::map<string, Channel *> &Server::getChannels()
{
	return _channels;
}

/**
 * @brief Get the Password object
 *
 * @return string
 */

string Server::getPassword() const
{
	return _password;
}

/**
 * @brief Set the Password object
 *
 * @param password
 */

void Server::setPassword(const string password)
{
	_password = password;
}

std::vector<pollfd> &Server::getPollFd()
{
	return (_pollfds);
}

/**
 * @brief Get the Data Time object
 *
 * @return string
 */

string Server::getDataTime() const
{
	std::ostringstream oss;
	struct tm *timeinfo = std::localtime(&_serverCreatedTime);
	char buffer[80];
	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	oss << buffer;
	return oss.str();
}

/**
 * @brief Set the Server Created Time object
 *
 */

void Server::setServerCreatedTime()
{
	_serverCreatedTime = std::time(0);
}

/**
 * @brief Get the Server Name object
 *
 * @return string
 */

string Server::getServerName() const
{
	return (_serverName);
}