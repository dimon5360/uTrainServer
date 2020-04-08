/**
 *  @file UUnitTests.h
 *  @note Unit tests class prototype
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 08.04.2020
 *  @version 0.1
 */

#pragma once

#include "config.h"

#if UNIT_TESTS_ENABLE

/* error codes for unit tests */
enum class err_type_ut {
    ERR_OK = 0,
    ERR_QUEUE_FAILED,
    ALL_CODES_MOUNT
};

class UUnitTests
{
private:
    bool UintTestsResult = true;

public:
    /* unit tests class constructor */
    UUnitTests();
    bool GetUnitTestsResult(void);
};
#endif /* UNIT_TESTS_ENABLE */

