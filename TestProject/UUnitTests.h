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
/* Unit tests defines ------------------------------------------------------ */
#define TEST_DATA_SIZE  2

/* error codes for unit tests ---------------------------------------------- */
enum class err_type_ut {
    ERR_OK = 0,
    ERR_QUEUE_FAILED,
    ERR_JSON_PARSER_FAILED,
    ERR_ALL_CODES_MOUNT
};

/* Unit tests class --------------------------------------------------------- */
class UUnitTests
{
private:
    /* Unit tests result */
    bool UintTestsResult = true;

public:
    /* Unit tests class constructor */
    UUnitTests();
    /* Function returns unit tests result */
    bool GetUnitTestsResult(void);
};
#endif /* UNIT_TESTS_ENABLE */

