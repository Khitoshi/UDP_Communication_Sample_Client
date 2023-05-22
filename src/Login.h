#pragma once

#include <grpcpp/grpcpp.h>
#include "./pb/login.grpc.pb.h"


class LoginClient {
public:
    LoginClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(login::LoginService::NewStub(channel)) {}

    std::string Login(const std::string& user);

private:
    std::unique_ptr<login::LoginService::Stub> stub_;
};