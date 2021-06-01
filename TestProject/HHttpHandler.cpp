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
#include <boost/beast/http/string_body.hpp>

/* data processor handler thread */
std::thread httpHandlerThread;

/**
 * @brief HTTP data hadnler class constructor
 */
HHttpHandler::HHttpHandler() {

    /* initialize JSON parser handler  ===================================== */
    jsonParser = std::make_unique<JJsonParser>();
    
    /* single thread for HTTP handler */
    /*httpHandlerThread = std::thread(&HHttpHandler::handle, this);
    httpHandlerThread.join();*/
    std::thread(&HHttpHandler::handle, this).detach();
    //handle();
}

/**
 * @brief HTTP data hadnler class destructor
 */
HHttpHandler::~HHttpHandler() {
}


/***
 * @brief Print info about input HTTP request
 */
static void PrintMethodInfo(boost::beast::http::request<boost::beast::http::string_body> parseResult) {

    switch (parseResult.method()) {

    case boost::beast::http::verb::get:
#if HTTP_PROC_PARSER_LOG
        cout << "================== GET ==================\n";
#endif /* HTTP_PROC_PARSER_LOG */
        break;
    case boost::beast::http::verb::post:
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

    std::string jsonResp, jsonReq;
    err_type_hh errCode = err_type_hh::ERR_OK;

#if HTTP_HANDLER_LOG
    ConsoleInfo("HTTP handler thread started.");
#endif /* HTTP_HANDLER_LOG */

    try {
        while (1) {
            /* if data processor input queue is not empty */
            if (!httpHandlerReqsQueueEmpty()) {
#if HTTP_HANDLER_LOG
                ConsoleLog("HTTP handler reqs queue is not empty.");
#endif /* HTTP_HANDLER_LOG */
                std::string httpReq;
                httpReq = pullHttpHandlerReqsQueue();
                errCode = procHttpRequest(std::move(httpReq));
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
                pushHttpHandlerRespsQueue(std::move(jsonResp));
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
        }
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}


/***
 * @brief Process HTTP request 
 */
err_type_hh HHttpHandler::procHttpRequest(std::string&& httpReq) {

    namespace http = boost::beast::http;
    /* unit tests error type */
    err_type_hh errCode = err_type_hh::ERR_OK;

    try {
        boost::beast::error_code ec;
        http::request_parser<http::string_body> parser;

        parser.eager(true);

        parser.put(boost::asio::buffer(std::move(httpReq)), ec);
        
        if (ec.value()) {
            std::cout << ec.message() << std::endl;
            return err_type_hh::ERR_HTTP_PARSER_FAILED;
        }

        http::request<http::string_body> parseResult = parser.get();
#if HTTP_PROC_PARSER_LOG
        std::cout << "HTTP request method: \n" << parseResult.method() << std::endl;
        std::cout << "HTTP request body: \n" << parseResult.body() << std::endl;
#endif /* HTTP_PROC_PARSER_LOG */

        /* Print information about input HTTP request */
        PrintMethodInfo(parseResult);

#if HTTP_PROC_PARSER_LOG
        cout << "// Resend to JSON parser.\n";
#endif /* HTTP_PROC_PARSER_LOG */
        std::string json_req = parseResult.body();
        jsonParser->pushJsonReqsQueue(std::move(json_req));
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

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
void HHttpHandler::pushHttpHandlerRespsQueue(std::string&& sHttpResponse) {
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        httpHandlerRespsQueue.push(sHttpResponse);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}

/***
 * @brief Get first response from data process class queue
 */
const std::string& HHttpHandler::pullHttpHandlerRespsQueue(void) {
    std::string httpResp;
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
            httpResp = httpHandlerRespsQueue.front();
            httpHandlerRespsQueue.pop();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    return httpResp;
}

/***
 * @brief Put new request in data process class queue
 */
void HHttpHandler::pushHttpHandlerReqsQueue(std::string&& sHttpRequest) {
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        httpHandlerReqsQueue.push(sHttpRequest);
    } 
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}

/***
 * @brief Get first request from data process class queue
 */
const std::string& HHttpHandler::pullHttpHandlerReqsQueue(void) {

    std::string httpReq;
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        httpReq = httpHandlerReqsQueue.front();
        httpHandlerReqsQueue.pop();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    return httpReq;
}