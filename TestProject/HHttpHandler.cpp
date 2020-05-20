/**
 *  @file HHttpHandler.cpp
 *  @note HTTP request and response handler class realization
 *
 *  @date 29.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 29.04.2020
 *  @version 0.1
 */

 /* main classes headers */
#include "main.h"

#include <boost/beast/http/parser.hpp> 
#include <boost_1_72_0_b1_rc2\boost\beast\http\string_body.hpp>

using namespace boost::beast::http;
namespace http = boost::beast::http;

/* data processor handler thread */
std::thread httpHandlerThread;

/**
 * @brief HTTP data hadnler class constructor
 */
HHttpHandler::HHttpHandler() {
#if HTTP_PROC_CONSTR_DESTR_LOG
    ConsoleInfo("HTTP handler class object created.");
#endif /* HTTP_PROC_CONSTR_DESTR_LOG */

    /* initialize JSON parser handler  ===================================== */
    jsonParser = make_shared<JJsonParser>();
    
    /* single thread for HTTP handler */
    httpHandlerThread = std::thread(&HHttpHandler::handle, this);
    httpHandlerThread.detach();
}

/**
 * @brief HTTP data hadnler class destructor
 */
HHttpHandler::~HHttpHandler() {
#if HTTP_PROC_CONSTR_DESTR_LOG
    ConsoleInfo("HTTP handler class object removed.");
#endif /* HTTP_PROC_CONSTR_DESTR_LOG */
    httpHandlerThread.~thread();
}


/***
 * @brief Print info about input HTTP request
 */
static void PrintMethodInfo(request<string_body> parseResult) {
#if HTTP_HANDLER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* HTTP_HANDLER_CALLED_FUNCTION */

    switch (parseResult.method()) {

    case http::verb::get:
#if HTTP_PROC_PARSER_LOG
        cout << "================== GET ==================\n";
#endif /* HTTP_PROC_PARSER_LOG */
        break;
    case http::verb::post:
#if HTTP_PROC_PARSER_LOG
        cout << "================== POST ==================\n";
#endif /* HTTP_PROC_PARSER_LOG */
        break;
    default:
        return;
    }

#if HTTP_PROC_PARSER_LOG
    /* this is not a log, need to realize parser for these data */
    std::cout << "HTTP method : " << parseResult.method() << std::endl;
    std::cout << "Host: " << parseResult.at("Host") << std::endl;
    std::cout << "Content-Type: " << parseResult.at("Content-Type") << std::endl;
    std::cout << "Accept: " << parseResult.at("Accept") << std::endl;
    std::cout << "Body: " << parseResult.body() << std::endl;
#endif /* HTTP_PROC_PARSER_LOG */
}

/**
 * @brief HTTP data handler
 * @note If server received data then data processor takes and
 *       gives it to JSON parser. After this it waits a response.
 *       Then response transfers to data base handler and
 *       data processor waits a response. After response will be transfered
 *       to JSON wrapper and transmited by TCP/IP channel to client.
 *
 */
void HHttpHandler::handle() {
#if HTTP_HANDLER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* HTTP_HANDLER_CALLED_FUNCTION */

    std::string jsonResp, jsonReq;
    err_type_hh errCode = err_type_hh::ERR_OK;

#if HTTP_HANDLER_LOG
    ConsoleInfo("HTTP handler thread started.");
#endif /* HTTP_HANDLER_LOG */

    while (1) {
        /* if data processor input queue is not empty */
        if (!httpHandlerReqsQueueEmpty()) {
#if HTTP_HANDLER_LOG
            ConsoleLog("HTTP handler reqs queue is not empty.");
#endif /* HTTP_HANDLER_LOG */
            std::string httpReq;
            httpReq = pullHttpHandlerReqsQueue();
            errCode = procHttpRequest(httpReq);
        }
        else if (!jsonParser->jsonRespsQueueEmpty()) {
#if HTTP_HANDLER_LOG
            ConsoleLog("JSON parser responses queue is not empty.");
#endif /* HTTP_HANDLER_LOG */
            jsonResp = jsonParser->pullJsonRespsQueue();
#if HTTP_HANDLER_LOG
            ConsoleLog("Need to send the answer " + string("\"") + 
                jsonResp + string("\" ") + "from JSON parser "
                "to HTTP handler queue.");
#endif /* HTTP_HANDLER_LOG */
            pushHttpHandlerRespsQueue(jsonResp);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
    }
}

/***
 * @brief Process HTTP request 
 */
err_type_hh HHttpHandler::procHttpRequest(string httpReq) {
#if HTTP_HANDLER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* HTTP_HANDLER_CALLED_FUNCTION */

    /* unit tests error type */
    err_type_hh errCode = err_type_hh::ERR_OK;


    boost::beast::error_code ec;
    request_parser<string_body> parser;

    parser.eager(true);
    
    parser.put(boost::asio::buffer(httpReq), ec);
    
    request<string_body> parseResult = parser.get();
#if HTTP_PROC_PARSER_LOG
    std::cout << "HTTP request method: \n" << parseResult.method() << std::endl;
    std::cout << "HTTP request body: \n" << parseResult.body() << std::endl;
#endif /* HTTP_PROC_PARSER_LOG */

    /* Print information about input HTTP request */
    PrintMethodInfo(parseResult);

#if HTTP_PROC_PARSER_LOG
    cout << "// Resend to JSON parser.\n";
#endif /* HTTP_PROC_PARSER_LOG */
    jsonParser->pushJsonReqsQueue(parseResult.body());

    return errCode;
}

/***
 * @brief Returns true if queue with requests is empty
 */
bool HHttpHandler::httpHandlerReqsQueueEmpty(void) {
    return httpHandlerReqsQueue.empty();
}

/***
 * @brief Returns true if queue with responses is empty
 */
bool HHttpHandler::httpHandlerRespsQueueEmpty(void) {
    return httpHandlerRespsQueue.empty();
}

/***
 * @brief Put response in data process class queue
 */
void HHttpHandler::pushHttpHandlerRespsQueue(std::string sHttpResponse) {
#if HTTP_HANDLER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* HTTP_HANDLER_CALLED_FUNCTION */
    httpHandlerRespsQueue.push(sHttpResponse);
}

/***
 * @brief Get first response from data process class queue
 */
std::string HHttpHandler::pullHttpHandlerRespsQueue(void) {
#if HTTP_HANDLER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* HTTP_HANDLER_CALLED_FUNCTION */
    std::string httpResp = "";
    httpResp = httpHandlerRespsQueue.front();
    httpHandlerRespsQueue.pop();
    return httpResp;
}

/***
 * @brief Put new request in data process class queue
 */
void HHttpHandler::pushHttpHandlerReqsQueue(std::string sHttpRequest) {
#if HTTP_HANDLER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* HTTP_HANDLER_CALLED_FUNCTION */
    httpHandlerReqsQueue.push(sHttpRequest);
}

/***
 * @brief Get first request from data process class queue
 */
std::string HHttpHandler::pullHttpHandlerReqsQueue(void) {
#if HTTP_HANDLER_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* HTTP_HANDLER_CALLED_FUNCTION */
    std::string httpReq = "";
    httpReq = httpHandlerReqsQueue.front();
    httpHandlerReqsQueue.pop();
    return httpReq;
}