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
#endif /* UNIT_TEST_DATA_PROCESSOR */

#endif /* UNIT_TESTS_ENABLE */