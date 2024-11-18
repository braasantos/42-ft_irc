#pragma once

#include "utils.hpp"
#include "helper.hpp"
#include "CommandHandler.hpp"

class Client;
class Channel;
class CommandHandler;
class Server
{
private:
	int _port;
	int _socketfd;
	string _password;
	string _serverName;
	sockaddr_in _server_sockIPV4;
	std::vector<pollfd> _pollfds;
	std::map<int, Client *> _clients;
	std::map<string, Channel *> _channels;
	CommandHandler *_commandHandler;
	time_t _serverCreatedTime;

public:
	Server();
	~Server();

	void createSocket(int port);
	void startServerIPV4();

	string getPassword() const;
	string getDataTime() const;
	string getServerName() const;

	Channel *getChannel(const string &channel_name);
	Client *getClient(const int &fd);
	std::map<string, Channel *> &getChannels();
	std::map<int, Client *> &getClients();

	void setPassword(const string password);

	string readFromSocket(int fd);
	void addClient(int socketfd);
	void addChannel(const string &channel_name, Channel *channel);
	void removeClient(int client_fd);
	void handleMessageFromClient(int fd);
	void setServerCreatedTime();
	std::vector<pollfd> &getPollFd();
	int getSocketFd();
	CommandHandler* getCommandHandler();
};