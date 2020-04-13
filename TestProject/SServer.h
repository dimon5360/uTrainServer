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

#include "DDataBase.h"
#include "config.h"

#include <memory>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <xstring>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <sstream>

/* tcp stack */
#if !NEW_REALIZATION
#include "winsock.h"
#pragma comment(lib, "ws2_32.lib")
#endif

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

public:
    /* public prototypes */
    SServer(std::string s_ip, uint16_t s_port);
    ~SServer();
};

/* main server thread handler */
extern void CreateNewThread(SOCKET s, SOCKADDR_IN addr);
