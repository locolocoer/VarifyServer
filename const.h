#pragma once
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <functional>
#include <map>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <queue>
namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = asio::ip::tcp;

enum ErrorCodes
{
	Sucess = 1000,
	ErrorJson = 1001,
	ErrorGrpc = 1002,
	VarifyExpired = 1003,
	VarifyError = 1004,
	UserAlreadyExist = 1005,
	usrNotMatchEmail = 1006,
	upPwdFailed = 1007,
	PwdNameNotMatch = 1008,
};


class UserInfo {
public:
	std::string email;
	int uid;
	std::string name;
	std::string pwd;
};

#define CODE_PREFIX "code_"