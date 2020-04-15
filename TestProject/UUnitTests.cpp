/**
 *  @file UUnitTests.cpp
 *  @note Unit tests class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 10.04.2020
 *  @version 0.3
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
/* Test of work JSON parser */
static err_type_ut TestJsonParserTestData(void);
#endif /* UNIT_TEST_JSON_PARSER */

/***
 * @brief Unit tests class constructor
 */
UUnitTests::UUnitTests() {
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

/**
 * @brief Initialize unit tests
 */
static err_type_ut UnitTestsInit(void) {
    err_type_ut err = err_type_ut::ERR_OK;

#if UNIT_TEST_JSON_PARSER
    err = TestExchangeQueue();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
    err = TestJsonParserTestData();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_JSON_PARSER */

    return err_type_ut::ERR_OK;
}

#if UNIT_TEST_JSON_PARSER
/**
 * @brief Test of work JSON queue
 */
static err_type_ut TestExchangeQueue(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    string sTestString = "abcdef123456789";

    /* test data processor in main thread */
    shared_ptr<JJsonParser> jsonProcessor = make_shared<JJsonParser>(sTestString, false);

    if (!jsonProcessor->GetUnitTestResult()) {
        errCode = err_type_ut::ERR_QUEUE_FAILED;
    }
    return errCode;
}

/**
 * @brief Test of work JSON parser
 */
static err_type_ut TestJsonParserTestData(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    string sJsonTestReq;

#if TEST1
    sJsonTestReq = "{ \"menu\": { \"foo\": [ \"ad \", 4, 3, 4, 5 ], \"bar\": \"true\", \"value\": 102.3E+06,\
                     \"popup\": { \"value\": \"New\", \"onclick\": \"CreateNewDoc()\" } } }";
#elif TEST2
    sJsonTestReq = "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }";
#elif TEST3
    sJsonTestReq = "{ \"foo\" : \"bar\" }";
#elif TEST4
    // TODO: HTTP request header, use it later
    /*"POST / something HTTP / 1.1"
    "Content - Type: application / json"
    "Accept : application / json"
    "Host : domain"
    "Content - Length : 216"*/
    sJsonTestReq = "{ \"username\": \"Dmitry\", \"password\": \"admin\", \"validation-factors\": {\
            \"validationFactors\" : { \"name\" : \"remote_address\", \"value\" : \"127.0.0.1\" } } }";

#endif /* TEST_ITEM */

#if UNIT_TESTS_LOG
    cout << sJsonTestReq << endl << endl;
#endif /* UNIT_TESTS_LOG */
    /* test data processor in main thread */
    shared_ptr<JJsonParser> jsonProcessor = make_shared<JJsonParser>(sJsonTestReq, true);

    if (!jsonProcessor->GetUnitTestResult()) {
        errCode = err_type_ut::ERR_JSON_PARSER_FAILED;
    }
    return errCode;
}
#endif /* UNIT_TEST_JSON_PARSER */

#endif /* UNIT_TESTS_ENABLE */