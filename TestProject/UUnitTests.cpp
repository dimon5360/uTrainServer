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
/* Initialize unit tests */
static err_type_ut UnitTestsInit(void);

#if UNIT_TEST_DATA_PROCESSOR
/* Test of handler HTTP request */
static err_type_ut HttpDataProcHandlerTest(void);
#endif /* UNIT_TEST_DATA_PROCESSOR */

/* Test of handler data base request */
static err_type_ut HttpReqParse(void);

/* User code --------------------------------------------------------------- */
/***
 * @brief Unit tests class constructor
 */
UUnitTests::UUnitTests(void) {
    err_type_ut err = err_type_ut::ERR_OK;
    err = UnitTestsInit();
    if (err != err_type_ut::ERR_OK) {
        std::cout << "Unit tests failed. Error code: " << 
            boost::format("%u") % (uint32_t)err << endl;
        UintTestsResult = false;
    }
    else {
        std::cout << "Unit tests succeed. Error code: " <<
            boost::format("%u") % (uint32_t)err << endl;
    }
}

/***
 * @brief Function returns unit tests result
 */
bool UUnitTests::GetUnitTestsResult(void) {
    return UintTestsResult;
}

/***
 * @brief Initialize unit tests
 */
static err_type_ut UnitTestsInit(void) {
    err_type_ut err = err_type_ut::ERR_OK;

#if UNIT_TEST_DATA_PROCESSOR
    /* Test HTTP handler */
    err = HttpDataProcHandlerTest();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }

#if UNIT_TESTS_LOG

#endif /* UNIT_TESTS_LOG */

#endif /* UNIT_TEST_DATA_PROCESSOR */

#if UNIT_TEST_HTTP_HANDLER
#if UNIT_TESTS_LOG
#endif /* UNIT_TESTS_LOG */
    err = HttpReqParse();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_HTTP_HANDLER */

#if UNIT_TESTS_LOG
    cout << "Unit test succeed\n";
#endif /* UNIT_TESTS_LOG */

    return err_type_ut::ERR_OK;
}

/* Data processor unit tests =============================================== */
#if UNIT_TEST_DATA_PROCESSOR
/***
 * @brief Test of handler data base request
 */
static err_type_ut HttpDataProcHandlerTest(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    shared_ptr<DDataProcess> dataProcessor =
        make_shared<DDataProcess>();

    string jsonReq = "{ \"active\": true, \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
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

    dataProcessor->pushDataProcReqsQueue(req.str());
    while (1) {
        if (!dataProcessor->dataProcRespsQueueEmpty()) {
            cout << "\nData processor response: " << "\"" <<
                dataProcessor->pullDataProcRespsQueue() << "\"" <<endl;
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
 * @brief Test of handler data base request
 */
static err_type_ut HttpReqParse(void) {
    /* unit tests error type */
    err_type_hh errCode;
    /* process database requests */
    shared_ptr<HHttpHandler> httpHandler =
        make_shared<HHttpHandler>();
    std::string jsonReq = "{\"name\": \"test_name\", \"surname\": \"test_surname\"}\r\n";

    std::stringstream req;
    req << "POST /cgi/message.php HTTP/1.1\r\n"
        << "Content-Length: " << jsonReq.size() << "\r\n"
        << "Content-Type: application/json; utf-8\r\n"
        << "Host: www.utrain.com\r\n"
        << "Accept: application/json\r\n"
        << "\r\n"
        << jsonReq;

    errCode = httpHandler->procHttpRequest(req.str());
    if(errCode != err_type_hh::ERR_OK)
        return err_type_ut::ERR_HTTP_HANDLER_PARSER;

    return err_type_ut::ERR_OK;
}
#endif /* UNIT_TEST_HTTP_HANDLER */

#endif /* UNIT_TESTS_ENABLE */