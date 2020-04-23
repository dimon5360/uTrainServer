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

 /* presprocessor configuration */
#include "config.h"
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
//static err_type_ut TestDataBaseConnection(void);
/* Test of handler data base request */
static err_type_ut TestDataProcHandler(void);
#endif /* UNIT_TEST_DATA_PROCESSOR */

/* User code --------------------------------------------------------------- */
/***
 * @brief Unit tests class constructor
 */
UUnitTests::UUnitTests() {
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

#if UNIT_TEST_JSON_PARSER
    /*  Test work of queue exchange */
#if UNIT_TESTS_LOG
    std::cout << "\n#1 queue exchange unit test." <<
        "--------------------------------------\n\n";
#endif /* UNIT_TESTS_LOG */
    err = TestExchangeQueue();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#if UNIT_TESTS_LOG
   cout << "Unit test succeed\n";
#endif /* UNIT_TESTS_LOG */

   /*  Test of parsing test data */
#if UNIT_TESTS_LOG
    std::cout << "\n#2 json parser unit test." <<
        "-----------------------------------------\n\n";
#endif /* UNIT_TESTS_LOG */
    err = TestJsonDataProcExchangeTestData();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#if UNIT_TESTS_LOG
    cout << "Unit test succeed\n";
#endif /* UNIT_TESTS_LOG */
#endif /* UNIT_TEST_JSON_PARSER */


#if UNIT_TEST_DATA_PROCESSOR
    /*  Test of connection to data base */
#if UNIT_TESTS_LOG
    std::cout << "\n#3 data base connection unit test." <<
        "--------------------------------\n\n";
#endif /* UNIT_TESTS_LOG */
    /*err = TestDataBaseConnection();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }*/
#if UNIT_TESTS_LOG
    cout << "Unit test succeed\n";
#endif /* UNIT_TESTS_LOG */

    /*  Test of data base handler */
#if UNIT_TESTS_LOG
    std::cout << "\n#4 data base request handler unit test." <<
        "---------------------------\n\n";
#endif /* UNIT_TESTS_LOG */
    err = TestDataProcHandler();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#if UNIT_TESTS_LOG
    cout << "Unit test succeed\n";
#endif /* UNIT_TESTS_LOG */
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
    string jsonReq = jsonProcessor->pullJsonReqQueue();

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
 * @brief Test of handler data base request
 */
static err_type_ut TestDataProcHandler(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    shared_ptr<DDataProcess> dataProcessor =
        make_shared<DDataProcess>();
    
    cout << "Put test data in queue.\n";
    string sJsonTestReq = "{ \"active\": true, \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
            \"validationFactor1\" : [ { \"name\" : \"remote_address\", \"value\" : \"127.0.0.1\" } ],\
            \"validationFactor2\" : { \"name\" : \"main_address\", \"value\" : \"192.168.122.1\" } } }";

    dataProcessor->pushDataProcReqQueue(sJsonTestReq);
    while (!dataProcessor->GetUnitTestResult());
    return errCode;
}

/***
 * @brief Test of connection to data base
 */
 //static err_type_ut TestDataBaseConnection(void) {
 //    /* unit tests error type */
 //    err_type_ut errCode = err_type_ut::ERR_OK;
 //    /* process database requests */
 //    shared_ptr<DDataProcess> dataProcessor = 
 //        make_shared<DDataProcess>();
 //
 //    if (!dataProcessor->GetUnitTestResult()) {
 //        errCode = err_type_ut::ERR_DATA_BASE_CONNECTION_FAILED;
 //    }
 //    return errCode;
 //}
#endif /* UNIT_TEST_DATA_PROCESSOR */

#endif /* UNIT_TESTS_ENABLE */