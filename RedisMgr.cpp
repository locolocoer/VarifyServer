#include "RedisMgr.h"
#include "ConfigMgr.h"
#include "ConfigMgr.h"
#include "Defer.h"
bool RedisMgr::connect(std::string ip, short port)
{
	_connect = redisConnect(ip.c_str(), port);
	if (_connect == NULL || _connect->err) {
		std::cout << "faied connect to redis server" << std::endl;
		return false;
	}
	return true;
}
bool RedisMgr::Auth(std::string pwd)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "AUTH &s", pwd.c_str());
	if (this->_reply->type == REDIS_REPLY_ERROR) {
		std::cout << "password error" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	else {
		std::cout << "auth sucess" << std::endl;
		freeReplyObject(this->_reply);
		return true;
	}
}
bool RedisMgr::Get(const std::string& key, std::string& value)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "GET %s", key.c_str());
	if (this->_reply == NULL) {
		std::cout << "get " << key << " failed" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	if (this->_reply->type != REDIS_REPLY_STRING) {
		std::cout << "get " << key << " failed" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	value = this->_reply->str;
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::Set(const std::string& key, const std::string& value)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "SET %s %s", key.c_str(), value.c_str());
	if (this->_reply == NULL) {
		std::cout << "Set " << key << ":" << value << " failed" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	if (!(this->_reply->type == REDIS_REPLY_STATUS && (strcmp(this->_reply->str, "OK") == 0 || strcmp(this->_reply->str, "ok") == 0))) {
		std::cout << "Set " << key << ":" << value << " failed" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::LPush(const std::string& key, const std::string& value)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "LPUSH %s %s", key.c_str(), value.c_str());
	if (this->_reply == NULL) {
		std::cout << "Set " << key << ":" << value << " failed" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	if (this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer <= 0) {
		std::cout << "Set " << key << ":" << value << " failed" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::LPop(const std::string& key, std::string& value)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "LPOP %s ", key.c_str());
	if (_reply == nullptr || _reply->type == REDIS_REPLY_NIL) {
		std::cout << "Execut command [ LPOP " << key << " ] failure ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	value = _reply->str;
	std::cout << "Execut command [ LPOP " << key << " ] success ! " << std::endl;
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::RPush(const std::string& key, const std::string& value)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "RPUSH %s %s", key.c_str(), value.c_str());
	if (NULL == this->_reply)
	{
		std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	if (this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer <= 0) {
		std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	std::cout << "Execut command [ RPUSH " << key << "  " << value << " ] success ! " << std::endl;
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::RPop(const std::string& key, std::string& value)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "RPOP %s ", key.c_str());
	if (_reply == NULL || _reply->type == REDIS_REPLY_NIL) {
		std::cout << "Execut command [ RPOP " << key << " ] failure ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	value = _reply->str;
	std::cout << "Execut command [ RPOP " << key << " ] success ! " << std::endl;
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::HSet(const std::string& key, const std::string& hkey, const std::string& value)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str());
	if (_reply == NULL || _reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] failure ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] success ! " << std::endl;
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	const char* argv[4];
	size_t argvlen[4];
	argv[0] = "HSET";
	argvlen[0] = 4;
	argv[1] = key;
	argvlen[1] = strlen(key);
	argv[2] = hkey;
	argvlen[2] = strlen(hkey);
	argv[3] = hvalue;
	argvlen[3] = hvaluelen;
	this->_reply = (redisReply*)redisCommandArgv(connect, 4, argv, argvlen);
	if (_reply == NULL || _reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] failure ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] success ! " << std::endl;
	freeReplyObject(this->_reply);
	return true;
}
std::string RedisMgr::HGet(const std::string& key, const std::string& hkey)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	const char* argv[3];
	size_t argvlen[3];
	argv[0] = "HGET";
	argvlen[0] = 4;
	argv[1] = key.c_str();
	argvlen[1] = key.length();
	argv[2] = hkey.c_str();
	argvlen[2] = hkey.length();
	this->_reply = (redisReply*)redisCommandArgv(connect, 3, argv, argvlen);
	if (this->_reply == NULL || this->_reply->type == REDIS_REPLY_NIL) {
		freeReplyObject(this->_reply);
		std::cout << "Execut command [ HGet " << key << " " << hkey << "  ] failure ! " << std::endl;
		return "";
	}
	std::string value = this->_reply->str;
	freeReplyObject(this->_reply);
	std::cout << "Execut command [ HGet " << key << " " << hkey << " ] success ! " << std::endl;
	return value;
}
bool RedisMgr::Del(const std::string& key)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "DEL %s", key.c_str());
	if (this->_reply == NULL || this->_reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ Del " << key << " ] failure ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	std::cout << "Execut command [ Del " << key << " ] success ! " << std::endl;
	freeReplyObject(this->_reply);
	return true;
}
bool RedisMgr::ExistKey(const std::string& key)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "exists %s", key.c_str());
	if (this->_reply == NULL || this->_reply->type != REDIS_REPLY_INTEGER || this->_reply->integer == 0) {
		std::cout << "Not Found [ Key " << key << " ]  ! " << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	std::cout << " Found [ Key " << key << " ] exists ! " << std::endl;
	freeReplyObject(this->_reply);
	return true;
}
void RedisMgr::Close()
{
	redisFree(_connect);
	_conn_pool->Close();
}
RedisMgr::RedisMgr() :_connect(NULL), _reply(NULL)
{
	auto& cfg = ConfigMgr::Inst();
	auto host = cfg["Redis"]["Host"];
	auto port = cfg["Redis"]["Port"];
	auto pwd = cfg["Redis"]["Passwd"];
	_conn_pool.reset(new RedisConPool(5, host.c_str(), atoi(port.c_str()), pwd.c_str()));
}

bool RedisMgr::SetWithExpire(const std::string& key, const std::string& value, int seconds)
{
	auto connect = _conn_pool->getConnection();
	Defer defer([this, &connect]() {
		_conn_pool->returnConnect(connect);
		});
	this->_reply = (redisReply*)redisCommand(connect, "SET %s %s EX %d", key.c_str(), value.c_str(), seconds);
	if (this->_reply == NULL) {
		std::cout << "set " << key << ":" << value << "with expired time failed" << std::endl;
		freeReplyObject(this->_reply);
		return false;
	}
	freeReplyObject(this->_reply);
	return true;
}
