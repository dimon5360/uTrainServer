/**
 *  @file HHttpHandler.h
 *  @note HTTP request and response handler class prototype
 *
 *  @date 29.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 29.04.2020
 *  @version 0.1
 */

#pragma once
 /* main classes headers */
#include "main.h"

 /* error codes for HTTP handler class ------------------------------------- */
enum class err_type_hh {
    ERR_OK = 0,
    ERR_HTTP_METHOD_UNEXPECTED,
    ERR_ALL_CODES_MOUNT
};


class HHttpHandler
{
private:
    /* queue for http requests keeping */
    std::queue<std::string> httpHandlerReqsQueue;
    /* queue for http responses keeping */
    std::queue<std::string> httpHandlerRespsQueue;


public:
    /* constructor */
    HHttpHandler();
    /* destructor */
    ~HHttpHandler();

    /* put response in http handler class queue */
    void pushHttpHandlerRespsQueue(std::string sDataProcRequest);
    /* get new response from http handler class queue */
    std::string pullHttpHandlerRespsQueue(void);
    /* put new request in http handler class queue */
    void pushHttpHandlerReqsQueue(std::string sDataProcRequest);
    /* get new request from http handler class queue */
    std::string pullHttpHandlerReqsQueue(void);

    /* check that requests queue is empty */
    bool httpHandlerReqsQueueEmpty(void);
    /* check that responses queue is empty */
    bool httpHandlerRespsQueueEmpty(void);

    /* Process HTTP request */
    err_type_hh procHttpRequest(string httpReq);

    void handle();
};