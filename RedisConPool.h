#pragma once
#include "const.h"
#include <hiredis.h>
class RedisConPool
{
public:
	RedisConPool(size_t poolsize, const char* host,int port, const char* pwd);
	~RedisConPool();
	redisContext* getConnection();
	void returnConnect(redisContext* connect);
	void Close();
private:
	std::condition_variable cv;
	std::atomic<bool> _b_stop;
	std::mutex _mutex;
	std::queue<redisContext*> connections;
};

