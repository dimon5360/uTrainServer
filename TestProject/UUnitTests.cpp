/**
 *  @file UUnitTests.cpp
 *  @note Unit tests class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 08.04.2020
 *  @version 0.1
 */

#include "main.h"
#include "stdint.h"
#include <boost/format.hpp> // boost library

#if UNIT_TESTS_ENABLE
/* Unit tests protorypes --------------------------------------------------- */
static err_type_ut UnitTestsInit(void);
static err_type_ut TestQueue(void);


bool UUnitTests::GetUnitTestsResult(void) {
    return UintTestsResult;
}


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

static err_type_ut UnitTestsInit(void) {
    err_type_ut err = err_type_ut::ERR_OK;

#if UNIT_TEST_QUEUE_EXCHANGE
    err = TestQueue();
    if (err != err_type_ut::ERR_OK) {
        return err;
    }
#endif /* UNIT_TEST_QUEUE_EXCHANGE */

    return err_type_ut::ERR_OK;
}

#if UNIT_TEST_QUEUE_EXCHANGE
#define TEST_DATA_SIZE  2
/**
 * @brief Test of work JSON queue
 */
static err_type_ut TestQueue(void) {
    /* unit tests error type */
    err_type_ut errCode = err_type_ut::ERR_OK;
    string testString1 = "abcdef123456789";

    /* test data processor in main thread */
    shared_ptr<JJsonParser> jsonProcessor = make_shared<JJsonParser>(testString1);

    if (!jsonProcessor->GetUnitTestResult()) {
        errCode = err_type_ut::ERR_QUEUE_FAILED;
    }
    return errCode;
}
#endif /* UNIT_TEST_QUEUE_EXCHANGE */

#endif /* UNIT_TESTS_ENABLE */