#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>
#include <sstream>
//#include "../lib/picojson-master/picojson.h"
#include <picojson-master/picojson.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1234
#define BUFFER_SIZE 1024



int main() {

    WSADATA wsaData;
    SOCKET clientSocket;
    //server�p�A�h���X(��M�p)
    struct sockaddr_in serverAddr;
    //client�p�A�h���X(���M�p)
    struct sockaddr_in clietAddr;
    //socket�֌W�̌��ʂ�����悤
    int result = 0;
    picojson::value v;
    try
    {
        result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        //winsock������
        if (result != 0) {
            throw "winsock�̏������Ɏ��s";
        }

        //�\�P�b�g����
        clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            WSACleanup();
            throw "socket�̐����Ɏ��s";
        }

        //server�̏���ݒ�
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(SERVER_PORT);
        // ���M�A�h���X��ݒ�
        InetPton(serverAddr.sin_family, TEXT(SERVER_IP), &serverAddr.sin_addr.S_un.S_addr);

        //client�̏���ݒ�
        clietAddr.sin_family = AF_INET;
        clietAddr.sin_port = 0;
        InetPton(clietAddr.sin_family, TEXT(SERVER_IP), &clietAddr.sin_addr.S_un.S_addr);

        result = bind(clientSocket, (sockaddr*)&clietAddr, sizeof(clietAddr));
        if (result != 0) {
            //std::cout << "bind error: " << WSAGetLastError() << std::endl;
            throw "bind error: ";
        }

        //�X�V����
        while (1) {
            //�f�[�^����
            std::string id,name,message;
            std::cout << "id����: ";
            std::cin >> id;
            std::cout << "���O����: ";
            std::cin >> name;
            std::cout << "���b�Z�[�W����(�I������ɂ�\"exit\"�Ɠ���): ";
            std::cin>>message;
            //json�`���ɕϊ�
            //std::string err = picojson::parse(v, "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}");
            std::ostringstream oss;
            oss << "{\"id\":\""
                << id
                << "\",\"name\":\""
                << name
                << "\",\"message\":\""
                << message
                << "\"}";
            std::string sendData = oss.str();

            // ���b�Z�[�W�̑��M
            result = sendto(clientSocket, sendData.c_str(), sendData.size(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            if (result == SOCKET_ERROR) {
                std::cout << WSAGetLastError()<<std::endl;
            }

            // "exit"�����͂��ꂽ��I��
            if (strcmp(message.c_str(), "exit") == 0)
                break;

            //
            //char recvMessage[BUFFER_SIZE];
            //int recvlen = sizeof(serverAddr);
            //result = recvfrom(clientSocket, recvMessage, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &recvlen);
            //if (result == INVALID_SOCKET) {
            //    std::cout << "recvfrom error: " << WSAGetLastError() << std::endl;
            //}
            ////std::cout <<"server: "<<recvMessage << std::endl;
            ////�����񂩂�json�`���ɕϊ�
            //std::string a = recvMessage;
            //std::string err = picojson::parse(v, a);
            //if (!err.empty()) {
            //    std::cerr << err << std::endl;
            //}
            //std::cout << "server message:" << std::endl;
            //std::cout << "name:" << v.get("name").get<std::string>() << std::endl;
            //std::cout << "message:" << v.get("message").get<std::string>() << std::endl << std::endl;
            //
        }

    }
    catch (const char* errmessage)
    {
        //error method
        std::cout << errmessage << ": " << WSAGetLastError() << std::endl;
    }

    //clean up method
    if (closesocket(clientSocket) == SOCKET_ERROR) {
        std::cout << "closesocket failed with error: "<< WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }
    WSACleanup();
    return 0;
}

