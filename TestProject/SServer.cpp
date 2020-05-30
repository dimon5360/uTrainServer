/**
 *  @file SServer.cpp
 *  @note TCP server class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 20.05.2020
 *  @version 0.2
 */

 /* server classe header */
#include "SServer.h"
/* for formatted output */
#include "utils.h"

#include <functional>

std::thread t[THREADS_MAX_NUMBER];
uint32_t countThread = 0;
std::thread tcpServerMainThread;

/**
 * @brief server class constructor
 */
SServer::SServer(std::string s_ip, uint16_t s_port) {
#if TCP_SERVER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* TCP_SERVER_CALLED_FUNCTION */

    cout << "TCP server IP : " << boost::format("%s:%u") %
        s_ip % s_port << endl;

    /* init tcp ip address */
    this->port = s_port;
    /* init tcp ip port */
    this->ip = s_ip;
       
    /* process database requests */
    dataProcessor = make_shared<DDataProcess>();

    err_type_server err = SServerInit();
    if (err != err_type_server::ERR_OK) {
        ConsoleError("Initialization of TCP server failed.");
        return;
    }
}

/**
 * @brief server class destructor
 */
SServer::~SServer() {
#if TCP_SERVER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* TCP_SERVER_CALLED_FUNCTION */

    tcpServerMainThread.~thread();
#if USE_BOOST 

#else 
    /* stop tcp socket server */
    closesocket(sock);
    WSACleanup();
#endif /* USE_BOOST */
    ConsoleInfo("Server was stoped. You can close the application.");
}

/**
 * @brief initialize TCP server 
 */
err_type_server SServer::SServerInit(void) {
#if TCP_SERVER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* TCP_SERVER_CALLED_FUNCTION */

    io_service service;

    ip::tcp::endpoint ep(ip::address::from_string("0.0.0.0"), 40400);

    ip::tcp::acceptor acc(service, ep);

    while (true) {
        socket_ptr sock(new ip::tcp::socket(service));
        acc.accept(*sock);
        boost::thread(boost::bind(&SServer::handle, this, sock));
        std::cout << "New client accepted." << std::endl;
        boost::this_thread::sleep_for(boost::chrono::microseconds(100));
    }
    return err_type_server::ERR_OK;
}


/**
 * @brief server socket handler 
 */
void SServer::handle(socket_ptr sock) {
#if TCP_SERVER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* TCP_SERVER_CALLED_FUNCTION */

    char data[512];
    while (true) {
        size_t len = sock->read_some(buffer(data));
        if (len > 0) {
            printf("Received msg: %s\n", data);
            dataProcessor->pushDataProcReqsQueue(std::string(data));
        }

        /*if (!dataProcessor->dataProcRespsQueueEmpty()) {
            std::string resp = dataProcessor->pullDataProcRespsQueue();
            std::cout << "Response: " << resp << std::endl;
        }*/
        boost::this_thread::sleep_for(boost::chrono::microseconds(100));
    }
}