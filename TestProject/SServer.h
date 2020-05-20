/**
 *  @file SServer.h
 *  @note TCP server class prototype
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 08.04.2020
 *  @version 0.1
 */

#pragma once
 /* main classes headers */
#include "main.h"

/* import for console log */
#include <iostream>
/* import for multythreading */
#include <thread>
#include <memory>
#include <chrono>
#include <sstream>

#define USE_BOOST 1

/* tcp stack */
#if USE_BOOST
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#else
#include "winsock.h"
#pragma comment(lib, "ws2_32.lib")
#endif /* !USE_BOOST */

/*  max number of threads */
#define THREADS_MAX_NUMBER  10

/* error codes for server work */
enum class err_type_server {
    ERR_OK = 0,
    ERR_INIT_WSA,
    ERR_INIT_SOCKET,
    ERR_SOCKET_BIND,
    ERR_SOCKET_LISTEN,
    ERR_CONNECT,
};

class SServer {
    
public:
    /* public variables */
    struct sockaddr_in addr;
    int opt = 1, addrlen = sizeof(addr);

private:
    /* private variables */
    SOCKET sock = INVALID_SOCKET, new_socket = INVALID_SOCKET;
    WSAData wData;
    uint16_t port = 0;
    std::string ip = "";
    char buffer[1024];

    /* private prototypes */
    err_type_server SServerInit(void);
    void handle(void);
    void processConnection(SOCKET s, SOCKADDR_IN addr);

public:
    /* public prototypes */
    SServer(std::string s_ip, uint16_t s_port);
    ~SServer();
};
