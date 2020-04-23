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
/* presprocessor configuration */
#include "config.h"
/* import data base class */
#include "DDataBase.h"
/* import JSON parser class */
#include "JJsonParser.h"

/* import data type string */
#include <string>
#include <cstdint>
/* import data queue */
#include <queue>
/* import unique pointer */
#include <memory>


 /* error codes for udata process class ------------------------------------ */
enum class err_type_dp {
    ERR_OK = 0,

    ERR_ALL_CODES_MOUNT
};

class DDataProcess {

private:
    /* queue for input requests */
    std::queue<std::string> inDataProcQueue;
    /* queue for output reesponses */
    std::queue<std::string> outDataProcQueue;

    /* pointer to data base processor class */
    std::shared_ptr<DDataBase> dataBaseProcessor;
    std::shared_ptr<JJsonParser> jsonParser;

public:
    /* data process handler */
    void handle();
    /* public prototypes */
    DDataProcess();
    ~DDataProcess();
    
    /* put new request in data process class queue */
    void pushDataProcReqQueue(std::string sDataProcRequest);
    /* get new request from data process class queue */
    std::string pullDataProcRespQueue(void);

    /* UNIT_TESTS ---------------------------------------------------------- */
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

