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
/* Test of handler data base request */
static err_type_ut TestDataProcHandler(void);
/* Test of handler data base request */
static err_type_ut JsonDataProcHandler(void);
#endif /* UNIT_TEST_DATA_PROCESSOR */
/* Test of handler data base request */
static err_type_ut HttpReqParse(void);

/* User code --------------------------------------------------------------- */
/***
 * @brief Unit tests class constructor
 */
UUnitTests::UUnitTests(void) {
    err_type_ut err = err_type_ut::ERR_OK;
#if UNIT_TESTS_LOG
    std::cout << "\nUnit-testing started.";
    std::cout << "\n=================================" <<
        "================================= \n";
#endif /* UNIT_TESTS_LOG */
    err = UnitTestsInit();
#if UNIT_TESTS_LOG
    std::cout << "\n=================================" <<
        "================================= \n";
#endif /* UNIT_TESTS_LOG */
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
    /*  Test of data base handler */
#if UNIT_TESTS_LOG
    std::cout << "\n#1 data processor request handler unit test." <<
        "---------------------------\n\n";
#endif /* UNIT_TESTS_LOG */
    err = TestDataProcHandler();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }

    /*  Json data handler */
#if UNIT_TESTS_LOG
    std::cout << "\n#2 data processor request handler unit test." <<
        "---------------------------\n\n";
#endif /* UNIT_TESTS_LOG */
    /*err = JsonDataProcHandler();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }*/

#endif /* UNIT_TEST_DATA_PROCESSOR */

#if UNIT_TEST_HTTP_HANDLER
    /*  HTTP request parser */
#if UNIT_TESTS_LOG
    std::cout << "\n#2 data processor request handler unit test." <<
        "---------------------------\n\n";
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
static err_type_ut TestDataProcHandler(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    shared_ptr<DDataProcess> dataProcessor =
        make_shared<DDataProcess>();
    
    string sJsonTestReq = "{ \"active\": true, \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
            \"validationFactor1\" : [ { \"name\" : \"remote_address\", \"value\" : \"127.0.0.1\" } ],\
            \"validationFactor2\" : { \"name\" : \"main_address\", \"value\" : \"192.168.122.1\" } } }";

    dataProcessor->pushDataProcReqsQueue(sJsonTestReq);
    while (1) {
        if (!dataProcessor->dataProcRespsQueueEmpty()) {
            cout << "\nData processor response: " << dataProcessor->pullDataProcRespsQueue() << endl;
            break;
        }
    }    
    return errCode;
}

/***
 * @brief Test of handler data base request
 */
static err_type_ut JsonDataProcHandler(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    shared_ptr<DDataProcess> dataProcessor =
        make_shared<DDataProcess>();

    string sJsonTestReq = "{ \"active\": true, \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
            \"validationFactor1\" : [ { \"name\" : \"remote_address\", \"value\" : \"127.0.0.1\" } ],\
            \"validationFactor2\" : { \"name\" : \"main_address\", \"value\" : \"192.168.122.1\" } } }";

    dataProcessor->pushDataProcReqsQueue(sJsonTestReq);
    while (1) {
        if (!dataProcessor->dataProcRespsQueueEmpty()) {
            cout << "\nData processor response: " << dataProcessor->pullDataProcRespsQueue() << endl;
            break;
        }
    }
    return errCode;
}
#endif /* UNIT_TEST_DATA_PROCESSOR */

/* HTTP handler unit tests ================================================= */
#if UNIT_TEST_HTTP_HANDLER

#define REQUEST_2 1
#define REQUEST_1 0
#define REQUEST_0 0
/***
 * @brief Test of handler data base request
 */
static err_type_ut HttpReqParse(void) {
    /* unit tests error type */
    err_type_hh errCode;
    /* process database requests */
    shared_ptr<HHttpHandler> httpHandler =
        make_shared<HHttpHandler>();

#if REQUEST_0
    /* example of first HTTP request */
    std::string first_request_example =
        "GET / HTTP/1.1\r\n"
        "Host: developer.mozilla.org\r\n"
        "Accept-Language: fr\r\n"
        "\r\n"
        "ayufgauysgf";
#elif REQUEST_1
    std::string first_request_example =
        "POST /cgi/message.php HTTP/1.1\r\n"
        "Content-Length: 5\r\n"
        "Host: www.utrain.com\r\n"
        "\r\n"
        "abcde";
#elif REQUEST_2
    std::string jsonReq = "{\"name\": \"test_name\", \"surname\": \"test_surname\"}\r\n";

    std::stringstream req;
    req << "POST /cgi/message.php HTTP/1.1\r\n"
        << "Content-Length: " << jsonReq.size() << "\r\n"
        << "Content-Type: application/json; utf-8\r\n"
        << "Host: www.utrain.com\r\n"
        << "Accept: application/json\r\n"
        << "\r\n"
        << jsonReq;
#endif /* REQUEST_ITER */

    errCode = httpHandler->procHttpRequest(req.str());
    if(errCode != err_type_hh::ERR_OK)
        return err_type_ut::ERR_HTTP_HANDLER_PARSER;

    return err_type_ut::ERR_OK;
}
#endif /* UNIT_TEST_HTTP_HANDLER */

#endif /* UNIT_TESTS_ENABLE */