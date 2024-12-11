#include <iostream>
#include <grpcpp/grpcpp.h>
#include "VarifyServiceImpl.h"
#include <boost/asio.hpp>
#include "const.h"
using grpc::Server;
using grpc::ServerBuilder;

void RunServer() {
	//std::string server_add = "127.0.0.1:50001";
	//VarifyServiceImpl service;
	//grpc::ServerBuilder builder;
	//builder.AddListeningPort(server_add, grpc::InsecureServerCredentials());
	//builder.RegisterService(&service);
	//std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	//std::cout << "Server listening on " << server_add << std::endl;
	//boost::asio::io_context ioc;
	//boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
	//signals.async_wait([&](boost::system::error_code ec, int signal_number) {
	//	if (!ec) {
	//		std::cout << "server shutdown!" << std::endl;
	//		server->Shutdown();
	//	}
	//	});
	//std::thread([&ioc]() {
	//	ioc.run();
	//	}).detach();
	//	server->Wait();
	//	ioc.stop();

	std::string address = "0.0.0.0:50051";
	grpc::ServerBuilder builder;
	builder.AddListeningPort(address, grpc::InsecureServerCredentials());
	VarifyServiceImpl service;
	builder.RegisterService(&service);
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	boost::asio::io_context ioc;
	boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
	signals.async_wait([&](boost::system::error_code ec, int signal_num) {
		if (!ec) {
			server->Shutdown();
		}
		});
	std::thread([&ioc]() {
		ioc.run();
		}).detach();
	server->Wait();
	ioc.stop();
}

int main(void)
{
	RunServer();
}