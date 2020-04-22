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

#include "main.h"

#if UNIT_TESTS_ENABLE
#include "stdint.h"
#include <boost/format.hpp> // boost library

/* Unit tests protorypes --------------------------------------------------- */
/* Initialize unit tests */
static err_type_ut UnitTestsInit(void);

#if UNIT_TEST_JSON_PARSER
/* Test of work JSON queue */
static err_type_ut TestExchangeQueue(void);
/* Test of work JSON parser with data processor */
static err_type_ut TestJsonDataProcExchangeTestData(void);
#endif /* UNIT_TEST_JSON_PARSER */

#if UNIT_TEST_DATA_PROCESSOR
/*  Test of connection to data base */
static err_type_ut TestDataBaseConnection(void);
/* Test of handler data base request */
static err_type_ut TestDataBaseHandler(void);
#endif /* UNIT_TEST_DATA_PROCESSOR */

/* User code --------------------------------------------------------------- */
/***
 * @brief Unit tests class constructor
 */
UUnitTests::UUnitTests() {
    err_type_ut err = err_type_ut::ERR_OK;
    std::cout << "\nUnit-testing started.";
    std::cout << "\n=================================" <<
        "================================= \n";
    err = UnitTestsInit();
    std::cout << "\n=================================" <<
        "================================= \n";
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

#if UNIT_TEST_JSON_PARSER

    std::cout << "\n#1 queue exchange unit test." <<
        "--------------------------------------\n\n";
    err = TestExchangeQueue();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
    else {
        cout << "Unit test succeed\n";
    }
    
    std::cout << "\n#2 json parser unit test." <<
        "-----------------------------------------\n\n";
    err = TestJsonDataProcExchangeTestData();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
    else {
        cout << "Unit test succeed\n";
    }
#endif /* UNIT_TEST_JSON_PARSER */

#if UNIT_TEST_DATA_PROCESSOR
    /*  Test of connection to data base */
    std::cout << "\n#3 data base connection unit test." <<
        "--------------------------------\n\n";
    err = TestDataBaseConnection();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
    else {
        cout << "Unit test succeed\n";
    }

    /*  Test of data base handler */
    std::cout << "\n#4 data base request handler unit test." <<
        "---------------------------\n\n";
    err = TestDataBaseHandler();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
    else {
        cout << "Unit test succeed\n";
    }
#endif /* UNIT_TEST_DATA_PROCESSOR */

    return err_type_ut::ERR_OK;
}


/* JSON parser unit tests ================================================== */
#if UNIT_TEST_JSON_PARSER
/***
 * @brief Test of work JSON queue
 */
static err_type_ut TestExchangeQueue(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    string sTestString = "abcdef123456789";

    /* test data processor in main thread */
    shared_ptr<JJsonParser> jsonProcessor = 
        make_shared<JJsonParser>();

    jsonProcessor->putJsonReqQueue(sTestString);
    string jsonReq = jsonProcessor->getJsonReqQueue();

    if (jsonReq != sTestString) {
        errCode = err_type_ut::ERR_QUEUE_FAILED;
    }
    return errCode;
}

/***
 * @brief Test of work JSON parser with data processor
 */
static err_type_ut TestJsonDataProcExchangeTestData(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;

    // TODO: HTTP request header, use it later
    /*"POST / something HTTP / 1.1"
    "Content - Type: application / json"
    "Accept : application / json"
    "Host : domain"
    "Content - Length : 216"*/
    string sJsonTestReq = "{ \"active\": true, \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
            \"validationFactor1\" : [ { \"name\" : \"remote_address\", \"value\" : \"127.0.0.1\" } ],\
            \"validationFactor2\" : { \"name\" : \"main_address\", \"value\" : \"192.168.122.1\" } } }";

    /* test data processor in main thread */
    shared_ptr<JJsonParser> jsonProcessor = make_shared<JJsonParser>();

    jsonProcessor->putJsonReqQueue(sJsonTestReq);

    jsonProcessor->jsonHandle();

    if (!jsonProcessor->GetUnitTestResult()) {
        errCode = err_type_ut::ERR_JSON_PARSER_FAILED;
    }
    return errCode;
}
#endif /* UNIT_TEST_JSON_PARSER */

/* Data processor unit tests =============================================== */
#if UNIT_TEST_DATA_PROCESSOR
/***
 * @brief Test of connection to data base
 */
static err_type_ut TestDataBaseConnection(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    shared_ptr<DDataProcess> dataProcessor = 
        make_shared<DDataProcess>();

    if (!dataProcessor->GetUnitTestResult()) {
        errCode = err_type_ut::ERR_DATA_BASE_CONNECTION_FAILED;
    }
    return errCode;
}

/***
 * @brief Test of handler data base request
 */
static err_type_ut TestDataBaseHandler(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    shared_ptr<DDataProcess> dataProcessor =
        make_shared<DDataProcess>("Dmitry");

    if (!dataProcessor->GetUnitTestResult()) {
        errCode = err_type_ut::ERR_DATA_BASE_CONNECTION_FAILED;
    }
    return errCode;
}
#endif /* UNIT_TEST_DATA_PROCESSOR */

#endif /* UNIT_TESTS_ENABLE */