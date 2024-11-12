#pragma once

#include "utils.hpp"
#include "helper.hpp"

class Channel;
class Server;

class Client
{
	private:
		int _fd;
		int _port;

		int _userlen;
		unsigned int _channelimit;
		string _nickname;
		string _username;
		string _realname;
		string _hostname;
		string _password;

		bool _banned;
		bool _authenticated;


		bool _hasPassword;
		bool _hasNickname;
		bool _hasUsername;

		std::vector<Channel*> invited_channels;

		Server* _server;

	public:
		Client(string hostname, uint16_t port, int fd, Server *server);
		~Client();

		int getFd() const;
		int getMode() const;
		int getUserLen() const;

		bool isAuthenticated() const;
		bool hasPassword() const;
		bool hasNickname() const;
		bool hasUsername() const;
		bool getBanned() const;


		string getNickname() const;
		string getUsername() const;
		string getRealname() const;
		string getHostname() const;
		string getPassword() const;
		unsigned int getChannelLimit() const;
		Channel* getChannel(string name);
		std::vector<Channel*> &getInvitedChannels();
		Server* getServer();

		void setNickname(string nickname);
		void setUsername(string username);
		void setRealname(string realname);
		void setHostname(string hostname);
		void setPassword(string password);
		void setBanned(bool banned);


		void authenticatePassword(bool authenticated);
		void authenticateNickname(bool authenticated);
		void authenticateUsername(bool authenticated);
		void setAuthenticated(bool authenticated);
		
		void response(string message);

};

std::ostream &operator<<(std::ostream &os, Client const &client);