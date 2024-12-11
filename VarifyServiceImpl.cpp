#include "VarifyServiceImpl.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "CSmtpSendMail.h"
#include <curl/curl.h>
#include "RedisMgr.h"
::grpc::Status VarifyServiceImpl::GetVarifyCode(::grpc::ServerContext* context, const::message::GetVarifyReq* request, ::message::GetVarifyRsp* response)
{
	std::string recv_mail = request->email();
	auto redismgr = RedisMgr::getInstance();
	std::string unique_str;
	if (!redismgr->Get(CODE_PREFIX + recv_mail, unique_str)) {
		auto unique_id = boost::uuids::random_generator()();
		unique_str = to_string(unique_id).substr(0, 4);
		redismgr->SetWithExpire(CODE_PREFIX + recv_mail, unique_str, 600);
	}
	curl_global_init(CURL_GLOBAL_ALL);
	CSmtpSendMail sendMail;
	sendMail.SetSmtpServer("flyingfry@163.com", "NEZJZENUWIPISZUD", "smtp.163.com", "465");
	sendMail.SetSendName("frychat");
	sendMail.SetSendMail("flyingfry@163.com");
	sendMail.AddRecvMail(recv_mail.c_str());
	sendMail.SetSubject("TEST");
	std::string send_msg = "the varifycode is:" + unique_str;
	sendMail.SetBodyContent(send_msg.c_str());
	sendMail.SendMail();
	response->set_error(0);
	response->set_email(recv_mail.c_str());
	response->set_code(unique_str.c_str());
	return grpc::Status::OK;
}
