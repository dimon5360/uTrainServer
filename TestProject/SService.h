/**
 *  @file SService.h
 *  @note Parent class for service (SServer, DataProcess etc.)
 *
 *  @date 30.05.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 30.05.2020
 *  @version 0.1
 */
#pragma once

#include <queue>
#include <string>
#include <iostream>

class SService
{
private:
    /* Queue to keep input requests */
    std::queue<std::string> reqsServiceQueue;
    /* Queue to keep output responses */
    std::queue<std::string> respsServiceQueue;

public:
    SService() {
        std::cout << "Constructing the service." << std::endl;
    }
    virtual ~SService() {
        std::cout << "Destructing the service." << std::endl;
    }

    /* interfaces for work with queue -------------------------------------- */
    /* Push the pesponse to queue */
    void pushRespsQueue(std::string);
    /* Push the request to queue */
    void pushReqsQueue(std::string);
    /* Get response from queue */
    std::string pullRespsQueue(void);
    /* Get request from queue */
    std::string pullReqsQueue(void);
    /* Check that requests queue is empty */
    bool isReqsQueueEmpty(void);
    /* Check that responses queue is empty */
    bool isRespsQueueEmpty(void);

};

