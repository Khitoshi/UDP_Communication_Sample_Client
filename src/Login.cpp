#include "Login.h"
#include <string>

using grpc::ClientContext;
using grpc::Status;
using login::LoginRequest;
using login::LoginResponse;

std::string LoginClient::Login(const std::string& user) {
    // �f�[�^��ݒ�
    LoginRequest request;
    request.set_name(user);

    // RPC���Ăяo���A�T�[�o����̃��X�|���X��҂�
    LoginResponse response;
    ClientContext context;

    Status status = stub_->Login(&context, request, &response);

    // RPC�������������ǂ������`�F�b�N
    if (status.ok()) {
        return response.id();
    }
    else {
        std::cout << status.error_code() << ": " << status.error_message()
            << std::endl;
        return "RPC failed";

    }
}