/**
 *  @file UUnitTests.cpp
 *  @note Unit tests class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 09.04.2020
 *  @version 0.2
 */

#include "main.h"
#include "stdint.h"
#include <boost/format.hpp> // boost library

#if UNIT_TESTS_ENABLE
/* Unit tests protorypes --------------------------------------------------- */
/* Initialize unit tests */
static err_type_ut UnitTestsInit(void);

#if UNIT_TEST_QUEUE_EXCHANGE
/* Test of work JSON queue */
static err_type_ut TestExchangeQueue(void);
#endif /* UNIT_TEST_QUEUE_EXCHANGE */

#if UNIT_TEST_JSON_PARSE
/* Test of work JSON parser */
static err_type_ut TestJsonParserTestData(void);
#endif /* UNIT_TEST_JSON_PARSE */

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

#if UNIT_TEST_QUEUE_EXCHANGE
    err = TestExchangeQueue();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_QUEUE_EXCHANGE */

#if UNIT_TEST_JSON_PARSE
    err = TestJsonParserTestData();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_JSON_PARSE */

    return err_type_ut::ERR_OK;
}

#if UNIT_TEST_QUEUE_EXCHANGE
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
#endif /* UNIT_TEST_QUEUE_EXCHANGE */

#if UNIT_TEST_JSON_PARSE
/**
 * @brief Test of work JSON parser
 */
static err_type_ut TestJsonParserTestData(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    string sJsonTestReq = 
    "{ \"menu\" : {\"foo\": true, \"bar\" : \"true\", \"value\" : 102.3E+06,\"popup\" : [ {\"value\": \"New\", \"onclick\" : \"CreateNewDoc()\"}, { \"value\": \"Open\", \"onclick\" : \"OpenDoc()\" },]}}";

    /* test data processor in main thread */
    shared_ptr<JJsonParser> jsonProcessor = make_shared<JJsonParser>(sJsonTestReq, true);

    if (!jsonProcessor->GetUnitTestResult()) {
        errCode = err_type_ut::ERR_JSON_PARSER_FAILED;
    }
    return errCode;
}
#endif /* UNIT_TEST_JSON_PARSE */

#endif /* UNIT_TESTS_ENABLE */