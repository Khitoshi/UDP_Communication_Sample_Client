#include "Login.h"
#include <string>

using grpc::ClientContext;
using grpc::Status;
using login::LoginRequest;
using login::LoginResponse;

std::string LoginClient::Login(const std::string& user) {
    // データを設定
    LoginRequest request;
    request.set_name(user);

    // RPCを呼び出し、サーバからのレスポンスを待つ
    LoginResponse response;
    ClientContext context;

    Status status = stub_->Login(&context, request, &response);

    // RPCが成功したかどうかをチェック
    if (status.ok()) {
        return response.id();
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
        return "RPC failed";

    }
}