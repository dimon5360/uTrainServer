/**
 *  @file SServer.h
 *  @note TCP server class prototype
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 20.05.2020
 *  @version 0.2
 */

#ifndef SSERVER_H
#define SSERVER_H

#pragma once
 /* main classes headers */
#include "main.h"

/* import for console log */
#include <iostream>
#include <sstream>
#include <memory>

#define USE_BOOST 1

/* tcp stack */
#if USE_BOOST
#include <boost/format.hpp> // boost library
/* import for multythreading */
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

using namespace boost::asio;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

#else
#include "winsock.h"
#pragma comment(lib, "ws2_32.lib")

/* import for multythreading */
#include <chrono>
#include <thread>
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
#if !USE_BOOST
    char buffer[1024];
#endif /* !USE_BOOST */

    std::shared_ptr<DDataProcess> dataProcessor;

    /* private prototypes */
    err_type_server SServerInit(void);
    void handle(socket_ptr sock);
    void processConnection(SOCKET s, SOCKADDR_IN addr);

public:
    /* public prototypes */
    SServer(std::string s_ip, uint16_t s_port);
    ~SServer();
};

#endif /* SSERVER_H */