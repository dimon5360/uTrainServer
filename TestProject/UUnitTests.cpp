/**
 *  @file UUnitTests.cpp
 *  @note Unit tests class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 22.04.2020
 *  @version 0.4
 */

 /* main classes headers */
#include "main.h"

#if UNIT_TESTS_ENABLE
#include "stdint.h"
#include <boost/format.hpp> // boost library

/* Unit tests protorypes --------------------------------------------------- */
/* Test of HTTP request handler */
static err_type_ut HttpDataProcHandlerTest(void);
/* Test of data base request handler */
static err_type_ut HttpReqParseTest(void);
/* Test of TCP server connection handler */
static err_type_ut GetHttpReqAndTransferToHandlerTest(void);

/* Unit tests result */
static bool UintTestsResult;


/* User code --------------------------------------------------------------- /
/***
 * @brief Function returns unit tests result
 */
bool UUnitTests::GetUnitTestsResult(void) {
    return UintTestsResult;
}

/***
 * @brief Initialize unit tests
 */
err_type_ut UUnitTests::UnitTestsInit(void) {
    UintTestsResult = false;
    err_type_ut err = err_type_ut::ERR_OK;

#if UNIT_TEST_DATA_PROCESSOR
    /* Test HTTP handler */
    err = HttpDataProcHandlerTest();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_DATA_PROCESSOR */

#if UNIT_TEST_HTTP_HANDLER
#if UNIT_TESTS_LOG
#endif /* UNIT_TESTS_LOG */
    err = HttpReqParseTest();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_HTTP_HANDLER */

#if UNIT_TEST_SERVER_DATA_PROC
    /* Test TCP server connection handler */
    err = GetHttpReqAndTransferToHandlerTest();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_SERVER_DATA_PROC */

    return err_type_ut::ERR_OK;
}

/* Data processor unit tests =============================================== */
#if UNIT_TEST_DATA_PROCESSOR
/***
 * @brief Test of data base request handler
 */
static err_type_ut HttpDataProcHandlerTest(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    std::shared_ptr<DDataProcess> dataProcessor =
        std::make_shared<DDataProcess>();

    std::string jsonReq = "{\"name\": \"test_name\", \"surname\": \"test_surname\"}\r\n";
    //std::string jsonReq = "{ \"active\": true, \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
            \"validationFactor1\" : [ { \"name\" : \"remote_address\", \"value\" : \"127.0.0.1\" } ],\
            \"validationFactor2\" : { \"name\" : \"main_address\", \"value\" : \"192.168.122.1\" } } }";

    std::stringstream req;
    /*req << "POST /cgi/message.php HTTP/1.1\r\n"
        << "Content-Length: " << jsonReq.size() << "\r\n"
        << "Content-Type: application/json; utf-8\r\n"
        << "Host: www.utrain.com\r\n"
        << "Accept: application/json\r\n"
        << "\r\n"
        << jsonReq;*/

    req << "GET /cgi/message.php HTTP/1.1\r\n"
        << "Content-Length: " << jsonReq.size() << "\r\n"
        << "Content-Type: application/json; utf-8\r\n"
        << "Host: www.utrain.com\r\n"
        << "Accept: application/json\r\n"
        << "\r\n"
        << jsonReq;

    dataProcessor->pushDataProcReqsQueue(std::move(req.str()));
    while (1) {
        if (!dataProcessor->dataProcRespsQueueEmpty()) {
            std::cout << "\nData processor response: " << "\"" <<
                dataProcessor->pullDataProcRespsQueue() << "\"" << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
    }
    return errCode;
}
#endif /* UNIT_TEST_DATA_PROCESSOR */

/* HTTP handler unit tests ================================================= */
#if UNIT_TEST_HTTP_HANDLER

/***
 * @brief Test of data base request handler
 */
static err_type_ut HttpReqParseTest(void) {
    /* unit tests error type */
    err_type_hh errCode;
    /* process database requests */
    std::unique_ptr<HHttpHandler> httpHandler =
        std::make_unique<HHttpHandler>();
    std::string jsonReq = "{\"name\": \"test_name\", \"surname\": \"test_surname\"}\r\n";
    //std::string jsonReq = "{ \"active\": true, \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
            \"validationFactor1\" : [ { \"name\" : \"remote_address\", \"value\" : \"127.0.0.1\" } ],\
            \"validationFactor2\" : { \"name\" : \"main_address\", \"value\" : \"192.168.122.1\" } } }";

    std::stringstream req;
    req << "POST /cgi/message.php HTTP/1.1\r\n"
        << "Content-Length: " << jsonReq.size() << "\r\n"
        << "Content-Type: application/json; utf-8\r\n"
        << "Host: www.utrain.com\r\n"
        << "Accept: application/json\r\n"
        << "\r\n"
        << jsonReq;

    errCode = httpHandler->procHttpRequest(std::move(req.str()));
    while (1) {
        if (!httpHandler->httpHandlerRespsQueueEmpty()) {
            std::cout << "\nData processor response: " << "\"" <<
                httpHandler->pullHttpHandlerRespsQueue() << "\"" << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
    }
    if(errCode != err_type_hh::ERR_OK)
        return err_type_ut::ERR_HTTP_HANDLER_PARSER;

    return err_type_ut::ERR_OK;
}
#endif /* UNIT_TEST_HTTP_HANDLER */

/* TCP server unit tests =================================================== */
#if UNIT_TEST_SERVER_DATA_PROC

/***
 * @brief Test of TCP server connection handler
 */
static err_type_ut GetHttpReqAndTransferToHandlerTest (void) {
    /* unit tests error type */
    //err_type_hh errCode;
    /* process database requests */
    std::shared_ptr<SServer> tcpServer =
        std::make_shared<SServer>("127.0.0.1", 40400);

    /* process database requests */
    /*shared_ptr<DDataProcess> dataProcessor =
        make_shared<DDataProcess>();*/

    /*while (1) {
        if (!dataProcessor->dataProcRespsQueueEmpty()) {
            cout << "\nData processor response: " << "\"" <<
                dataProcessor->pullDataProcRespsQueue() << "\"" << endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
    }*/
    return err_type_ut::ERR_OK;
}
#endif /* UNIT_TEST_SERVER_DATA_PROC */

#endif /* UNIT_TESTS_ENABLE */