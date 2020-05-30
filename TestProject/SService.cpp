/**
 *  @file SService.cpp
 *  @note Parent class for service (SServer, DataProcess etc.) realization
 *
 *  @date 30.05.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 30.05.2020
 *  @version 0.1
 */

#include "config.h"
#include "main.h"

/* interfaces for work with queue ------------------------------------------ */
/***
 * @brief Push the pesponse to queue 
 */
void SService::pushRespsQueue(std::string response) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
    respsServiceQueue.push(response);
}

/***
 * @brief Push the request to queue 
 */
void SService::pushReqsQueue(std::string request) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
    reqsServiceQueue.push(request);
}

/***
 * @brief Get response from queue 
 */
std::string SService::pullRespsQueue(void) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
    std::string response;
    response = respsServiceQueue.front();
    respsServiceQueue.pop();
    return response;
}

/***
 * @brief Get request from queue 
 */
std::string SService::pullReqsQueue(void) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
    std::string request;
    request = reqsServiceQueue.front();
    reqsServiceQueue.pop();
    return request;
}

/***
 * @brief Check that requests queue is empty
 */
bool SService::isReqsQueueEmpty(void) {
    return reqsServiceQueue.empty();
}
/***
 * @brief Check that responses queue is empty
 */
bool SService::isRespsQueueEmpty(void) {
    return respsServiceQueue.empty();
}