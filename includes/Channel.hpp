#pragma once

#include "utils.hpp"

class Client;

class Channel
{
private:
	string _topic;
	string _creator_name;
	string _topic_setter;
	char _mode;

	string _key;
	int _userLimit;


	std::vector<string> _operators;
	std::vector<string> _invited_members;

	std::map<string, Client *> _members;
	std::map<string, Client *> _banList;

public:
	Channel();
	~Channel();
	string getName() const;
	std::vector<Client *> getMembers() const;
	std::map<string, Client*> &getBanlist();

	char getMode() const;
	int getUserlimit() const;
	string getTopic() const;
	bool isOperator(Client *client) const;
	bool isOnChannel(string nickname) const;
	bool isInviteOnly() const;
	void addMember(Client *client);

	void setMode(char mode);
	void unsetMode(char mode);
	void setKey(string key);
	void unsetKey();
	void addOperator(Client *client);
	void removeOperator(Client *client);
	void setUserLimit(int limit);
	void unsetUserLimit();
	void removeMember(Client *client);

};