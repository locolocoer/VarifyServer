#pragma 
#include "Singleton.h"
#include <hiredis.h>
#include "RedisConPool.h"
class RedisMgr:public Singleton<RedisMgr>
{
	friend class Singleton<RedisMgr>;
public:
	bool connect(std::string ip, short port);
	bool Auth(std::string pwd);
	bool Get(const std::string& key,std::string& value);
	bool Set(const std::string& key, const std::string& value);
	bool SetWithExpire(const std::string& key, const std::string& value, int seconds);
	bool LPush(const std::string& key, const std::string& value);
	bool LPop(const std::string& key, std::string& value);
	bool RPush(const std::string& key, const std::string& value);
	bool RPop(const std::string& key, std::string& value);
	bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
	bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);
	std::string HGet(const std::string& key, const std::string& hkey);
	bool Del(const std::string& value);
	bool ExistKey(const std::string& key);
	void Close();

private:
	std::unique_ptr<RedisConPool> _conn_pool;
	redisContext* _connect;
	redisReply* _reply;
	RedisMgr();
};

