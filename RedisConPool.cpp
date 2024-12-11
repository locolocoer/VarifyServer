#include "RedisConPool.h"

RedisConPool::RedisConPool(size_t poolsize, const char* host,int port, const char* pwd):_b_stop(false)
{
	for (size_t i = 0; i < poolsize; i++) {
		auto* context = redisConnect(host, port);
		if (context == nullptr || context->err != 0) {
			if (context != nullptr) {
				redisFree(context);
			}
			continue;
		}
		auto reply = (redisReply*)redisCommand(context, "AUTH %s", pwd);
		if (reply->type == REDIS_REPLY_ERROR) {
			std::cout << "AUTH FAILED" << std::endl;
			freeReplyObject(reply);
			continue;
		}
		freeReplyObject(reply);
		connections.push(context);
	}
}

RedisConPool::~RedisConPool()
{
	std::unique_lock<std::mutex> lock(_mutex);
	while (!connections.empty()) {
		connections.pop();
	}
}

redisContext* RedisConPool::getConnection()
{
	std::unique_lock<std::mutex> lock(_mutex);
	cv.wait(lock, [this]() {
		if (_b_stop) {
			return true;
		}
		return !connections.empty();
		});
	if (_b_stop) {
		return nullptr;
	}
	auto context = connections.front();
	connections.pop();
	return context;
}

void RedisConPool::returnConnect(redisContext* connect)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (_b_stop) {
		return;
	}
	connections.push(connect);
	cv.notify_one();
}

void RedisConPool::Close()
{
	_b_stop = true;
	cv.notify_all();
}
