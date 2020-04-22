/**
 * @file DDataProcess.h
 * @note Data processor class prototype
 *
 * @date 08.04.2020
 * @author Kalmykov Dmitry
 *
 * @modified 08.04.2020
 * @version 0.1
 */ 

#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <queue>


class DDataProcess {

private:

public:
    /* public prototypes */
    DDataProcess();
    DDataProcess(std::string sDatabaesRequest);
    ~DDataProcess();

    /* UNIT_TESTS -------------------------------------------------------------- */
private:
#if UNIT_TEST_DATA_PROCESSOR
    bool unitTestResult = false;
#endif /* UNIT_TEST_DATA_PROCESSOR */

public:
#if UNIT_TEST_DATA_PROCESSOR
    /* unit test result for test queue */
    bool GetUnitTestResult(void);
#endif /* UNIT_TEST_DATA_PROCESSOR */
};

