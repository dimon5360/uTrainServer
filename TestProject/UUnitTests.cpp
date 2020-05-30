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
#include "SServer.h"
#include "LLogger.h"

#if UNIT_TESTS_ENABLE
#include "stdint.h"
#include <boost/format.hpp> // boost library

/* Unit tests protorypes --------------------------------------------------- */
/* Initialize unit tests */
static err_type_ut UnitTestsInit(void);
/* Test of HTTP request handler */
static err_type_ut HttpDataProcHandlerTest(void);
/* Test of TCP server connection handler */
static err_type_ut TestAsyncTcpServerBasedOnBoost(void);
/* Test of async logger */
static err_type_ut CreateAsyncLogger(void);

/* User code --------------------------------------------------------------- */
/***
 * @brief Unit tests class constructor
 */
UUnitTests::UUnitTests(void) {
    err_type_ut err = err_type_ut::ERR_OK;
    stringstream unitTestsResult;

    err = UnitTestsInit();
    if (err != err_type_ut::ERR_OK) {
        unitTestsResult << "Unit tests failed. Error code: " <<
            boost::format("%u") % (uint32_t)err;
        ConsoleError(unitTestsResult.str());
        UintTestsResult = false;
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
#endif /* UNIT_TEST_DATA_PROCESSOR */

#if UNIT_TEST_SERVER_DATA_PROC
    /* Test TCP server connection handler */
    err = TestAsyncTcpServerBasedOnBoost();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_SERVER_DATA_PROC */

#if UNIT_TEST_LOGEER_CONSTRUCTOR
    /* Test TCP server connection handler */
    err = CreateAsyncLogger();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_LOGEER_CONSTRUCTOR */



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

/* TCP server unit tests =================================================== */
#if UNIT_TEST_SERVER_DATA_PROC

/***
 * @brief Test of TCP server connection handler
 */
static err_type_ut TestAsyncTcpServerBasedOnBoost (void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    /* process database requests */
    shared_ptr<SServer> tcpServer =
        make_shared<SServer>("127.0.0.1", 40400);
    return errCode;
}
#endif /* UNIT_TEST_SERVER_DATA_PROC */

/* Logger unit tests ======================================================= */
#if UNIT_TEST_LOGEER_CONSTRUCTOR
/***
 * @brief Test of async logger
 */
[[maybe_unused]] static err_type_ut CreateAsyncLogger(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    std::shared_ptr<LLogger> logger = std::make_shared<LLogger>();
    return errCode;
}
#endif /* UNIT_TEST_LOGEER_CONSTRUCTOR */

#endif /* UNIT_TESTS_ENABLE */