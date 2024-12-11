#pragma once
#include <grpcpp/grpcpp.h>
#include "msg.grpc.pb.h"

using grpc::Server;
using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;
class VarifyServiceImpl final:public VarifyService::Service
{
public:
	::grpc::Status GetVarifyCode(::grpc::ServerContext* context, const ::message::GetVarifyReq* request, ::message::GetVarifyRsp* response) override;
};

