#include "SServer.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <sstream>

std::thread t[THREADS_MAX_NUMBER];
uint32_t countThread = 0;

/**
 * @brief initialize TCP server 
 */
err_type SServer::SServerInit(void) {
#if LOG_FUNCTIONS_CALLS
    cout << __FUNCTION__ << endl;
#endif

    /* init winsock dll */
    if (WSAStartup(MAKEWORD(2, 2), &wData) != 0) {
        return err_type::ERR_INIT_WSA;
    }
    printf("WSA Startup succes\n");

    /* socket init */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        std::cout << "Error socket opening. Socket is " << sock << std::endl;
        return err_type::ERR_INIT_SOCKET;
    } 

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&addr,
        sizeof(addr)) < 0) {
        std::cout << "Socket did not bind.\n";
        return err_type::ERR_SOCKET_BIND;
    }

    if (listen(sock, THREADS_MAX_NUMBER) < 0) {
        std::cout << "Socket did not listen.\n";
        return err_type::ERR_SOCKET_LISTEN;
    }

    handle();
    return err_type::ERR_OK;
}


/**
 * @brief server socket handler 
 */
void SServer::handle() {
#if LOG_FUNCTIONS_CALLS
    cout << __FUNCTION__ << endl;
#endif
    SOCKET acceptS[THREADS_MAX_NUMBER] = { INVALID_SOCKET }, tempSock;
    uint32_t sockCount = 0;
    SOCKADDR_IN addr_c;
    char buf[1024] = { 0 };
    while (true)
    {
        int addrlen = sizeof(addr_c);
        if ((tempSock = accept(sock, (struct sockaddr*) &addr_c, &addrlen)) != SOCKET_ERROR
            && sockCount < THREADS_MAX_NUMBER) {
            acceptS[sockCount] = tempSock;
            printf("send\n");
            printf("sended Client connected from %u.%u.%u.%u:%u\n",
                (unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b1,
                (unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b2,
                (unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b3,
                (unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b4,
                ntohs(addr_c.sin_port));

            /* create thread for each tcp client */
            t[countThread] = std::thread( CreateNewThread, acceptS[sockCount], addr_c );
            /* thread exists while server works */
            t[countThread++].detach();
            std::cout << "Client #" << sockCount++ << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

/**
 * @brief create new thread for new client
 */
void CreateNewThread(SOCKET s, SOCKADDR_IN addr) {
#if LOG_FUNCTIONS_CALLS
    cout << __FUNCTION__ << endl;
#endif
    char buf[1024] = { 0 };
    int recvLen = 0;
    std::cout << std::this_thread::get_id() << std::endl;

    while (true) {
        if ((recvLen = recv(s, buf, 1024, 0)) > 0) {
            std::stringstream resp;
            std::stringstream resp_body;

            std::cout << "========================================\n";
            std::cout << "HTTP request: \n";
            std::cout << std::string(buf) << std::endl;

            if (strcmp(buf, (char*)"admin") == 0) {
                resp << "User found.";
                send(s, resp.str().c_str(), (int)resp.str().length(), 0);
            }
            else if (strcmp(buf, (char*)"password") == 0) {
                resp << "User autorized.";
                send(s, resp.str().c_str(), (int)resp.str().length(), 0);
                break;
            }
            else {
                /* prepare test html page for new client */
                std::cout << "========================================\n";
                std::cout << "HTTP response: \n";
                resp_body << "<title>Test C++ HTTP Server</title>\n"
                    << "<h1>Test page</h1>\n"
                    << "<p>This is body of the test page...</p>\n"
                    << "<h2>Request headers</h2>\n"
                    << "<pre>" << buf << "</pre>\n"
                    << "<em><small>Test C++ Http Server</small></em>\n";

                resp << "HTTP/1.1 200 OK\r\n"
                    << "Version: HTTP/1.1\r\n"
                    << "Content-Type: application/json\r\n"
                    << "Content-Length: " << resp_body.str().length()
                    << "\r\n\r\n"
                    << resp_body.str();
                //resp << "Unknown user.";
                std::cout << resp.str() << std::endl;
                send(s, resp.str().c_str(), (int)resp.str().length(), 0);
            }

            memset(buf, 0x00, 1024);

             /* send test html page */
             //send(s, resp.str().c_str(), (int)resp.str().length(), 0);
             std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    while (true) {
        if ((recvLen = recv(s, buf, 1024, 0)) > 0) {
            std::stringstream resp;
            std::stringstream resp_body;

            std::cout << std::string(buf) << std::endl;

            send(s, buf, recvLen, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

/**
 * @brief server class constructor 
 */
SServer::SServer(std::string s_ip, uint16_t s_port) {
#if LOG_FUNCTIONS_CALLS
    cout << __FUNCTION__ << endl;
#endif
    /* init tcp ip address */
    this-> port = s_port;
    /* init tcp ip port */
    this->ip = s_ip;
    err_type err = SServerInit();
    if (err != err_type::ERR_OK) {
        std::cout << "Initialization of TCP server failed." << std::endl;
    }
}

/**
 * @brief server class destructor
 */
SServer::~SServer() {
#if LOG_FUNCTIONS_CALLS
    cout << __FUNCTION__ << endl;
#endif
    /* stop tcp socket server */
    closesocket(sock);
    WSACleanup();
    std::cout << "Server was stoped. You can close app" << std::endl;
}
