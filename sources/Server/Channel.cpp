#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"

/**
 * @brief Construct a new Channel:: Channel object
 */
Channel::Channel() : _mode(0), _userLimit(2), _users(0)
{
	cout << GREEN << "Channel object created." << RESET << endl;
}

/**
 * @brief Destroy the Channel:: Channel object
 */
Channel::~Channel()
{
	cout << RED << "Channel object destroyed." << RESET << endl;
}

/**
 * @brief Get the Name object
 *
 * @return string
 */
string Channel::getName() const
{
	return (_topic);
}

/**
 * @brief Get the Members object
 *
 * @return std::vector<Client *>
 */
std::vector<Client *> Channel::getMembers() const
{
	std::vector<Client *> members;
	for (std::map<string, Client *>::const_iterator it = _members.begin(); it != _members.end(); ++it)
		members.push_back(it->second);

	return (members);
}

/**
 * @brief Get the Mode object
 *
 * @param mode
 * @return char
 */
char Channel::getMode() const
{
	return (_mode);
}

/**
 * @brief Check if the users limits and increment when one join
 *
 * @param client
 */
int Channel::incrementUsers()
{
	if (this->_users >= this->_userLimit)
	{
		cout << "CANNOT ADD MORE USERS\r\n";
		return 1;
	}
	this->_users += 1;
	return 0;
}

int Channel::decrementUsers()
{
	if (this->_users == 0)
	{
		cout << "CANNOT REMOVE MORE USERS\r\n";
		return 1;
	}
	this->_users -= 1;
	return 0;
}
/**
 * @brief Add a Member object
 *
 * @param client
 */
int Channel::addMember(Client *client)
{
	if (this->incrementUsers())
		return 1;
	_members[client->getNickname()] = client;
	return 0;
}

/**
 * @brief Check if the client is an operator
 *
 * @param client
 * @return true
 * @return false
 */

bool Channel::isOperator(Client *client) const
{
	std::vector<string>::const_iterator it = std::find(_operators.begin(), _operators.end(), client->getNickname());
	return (it != _operators.end());
}

/**
 * @brief Check if the client is on the channel
 *
 * @param nickname
 * @return true
 * @return false
 */

bool Channel::isOnChannel(string nickname) const
{
	return _members.find(nickname) != _members.end();
}

/**
 * @brief Check if the channel is invite only
 *
 * @return true
 * @return false
 */

bool Channel::isInviteOnly() const
{
	return (_mode == 'i');
}

/**
 * @brief Get the Topic object
 *
 * @return string
 */

string Channel::getTopic() const
{
	return (_topic);
}

/**
 * @brief Set the Mode object
 *
 * @param mode
 */

void Channel::setMode(char mode)
{
	_mode |= mode;
}

/**
 * @brief Unset the Mode object
 *
 * @param mode
 */

void Channel::unsetMode(char mode)
{
	_mode &= ~mode;
}

/**
 * @brief Set the Key object
 *
 * @param key
 */

void Channel::setKey(string key)
{
	_key = key;
}

/**
 * @brief Unset the Key object
 */

void Channel::unsetKey()
{
	_key.clear();
}

/**
 * @brief Add an Operator object
 *
 * @param client
 */

void Channel::addOperator(Client *client)
{
	_operators.push_back(client->getNickname());
}

/**
 * @brief Remove an Operator object
 *
 * @param client
 */

void Channel::removeOperator(Client *client)
{
	_operators.erase(std::remove(_operators.begin(), _operators.end(), client->getNickname()), _operators.end());
}

/**
 * @brief Set the User Limit object
 *
 * @param limit
 */

void Channel::setUserLimit(int limit)
{
	cout << "limit 1 " << _userLimit << endl;
	_userLimit = limit;
	cout << "limit 2 " << _userLimit << endl;
}

/**
 * @brief Unset the User Limit object
 */

void Channel::unsetUserLimit()
{
	int num = this->_userLimit;
	this->_userLimit = num;
}

/**
 * @brief Remove a member from the channel
 *
 * @param client
 */
int Channel::removeMember(Client *client)
{
	if (this->decrementUsers())
		return 1;
	_members.erase(client->getNickname());
	return 0;
}

std::map<string, Client*> &Channel::getBanlist()
{
	return(_banList);
}

int Channel::getUserlimit() const
{
	return (_userLimit);
}