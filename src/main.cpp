#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <tchar.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1234
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    std::string sendMessage;
    try
    {

        //winsock������
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
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
        //serverAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
        InetPton(serverAddr.sin_family, TEXT(SERVER_IP), &serverAddr.sin_addr.S_un.S_addr);
        
        //�X�V����
        while (1) {
            printf("���b�Z�[�W����͂��Ă������� (�I������ɂ�\"exit\"�Ɠ���): ");
            std::cin >> sendMessage;

            // ���b�Z�[�W�̑��M
            int status = sendto(clientSocket, sendMessage.c_str(), sendMessage.size(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            if (status == SOCKET_ERROR) {
                std::cout << WSAGetLastError()<<std::endl;
            }
            // "exit"�����͂��ꂽ��I��
            if (strcmp(sendMessage.c_str(), "exit") == 0)
                break;
        }

    }
    catch (const char* errmessage)
    {
        std::cout << errmessage << ": " << WSAGetLastError() << std::endl;
    }

    if (closesocket(clientSocket) == SOCKET_ERROR) {
        std::cout << "closesocket failed with error: "<< WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }
    WSACleanup();
    return 0;
}